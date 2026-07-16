#include <assert.h>
#include <stddef.h>

#include "x86/instr.h"

void encode_modrm(instr_t* instruction) {
    uint8_t modrm = 0;
    uint8_t mod;
    uint8_t reg;
    uint8_t rm;

    /*
        MODRM.mod encodes if we have register
        or memory operation with special displacement
        and indexing logic:

        0b00: [r/m]
        0b01: [r/m + disp8]
        0b10: [r/m + disp32]
        0b11: register
    */

    uint8_t opn = 0;
    const operand_t* reg_op = NULL;
    const operand_t* rm_op = NULL;
    switch (instruction->form->form) {
        case RM:
            opn = 1;
            reg_op = &instruction->operands[0];
            rm_op = &instruction->operands[1];
            break;
        case MR:
            opn = 0;
            rm_op = &instruction->operands[0];
            reg_op = &instruction->operands[1];
            break;
        case MI:
            opn = 0;
            rm_op = &instruction->operands[0];
            break;
        case M:
            opn = 0;
            rm_op = &instruction->operands[0];
            break;
        case I:
        case O:
        case ZO:
        case D:
            return;
        default:
            assert(0 && "Invalid instruction form");
            break;
    }

    if (rm_op->kind == OPK_REG) {
        /* Accumulator forms do not need a MODRM byte */
        if (instruction->form->mods & ENCM_ACC_OPCODE &&
            rm_op->reg == REG_RAX) {
            return;
        } else {
            mod = 0b11;
            rm = REG_LOW3(rm_op->reg);
        }
    } else if (rm_op->kind == OPK_MEM) {
        const int32_t disp = instruction->operands[opn].disp;

        if (rm_op->base == REG_INVD) {
            /* Absolute and relative addressing */
            instruction->dispsize = DISP0;
            mod = 0b00;
        } else if (disp == 0 && REG_LOW3(rm_op->base) != 5) {
            /* For RBP and R13 I need disp8 = 0 */
            mod = 0b00;
        } else if (disp < INT8_MAX && disp > INT8_MIN) {
            instruction->dispsize = DISP8;
            mod = 0b01;
        } else if (disp < INT32_MAX && disp > INT32_MIN) {
            instruction->dispsize = DISP32;
            mod = 0b10;
        } else {
            assert(0 && "Impossible MODRM.mod value!");
        }

        /* If SIB is used, set MODRM.rm to 0x04 */
        if (rm_op->index != REG_INVD ||
            (rm_op->base != REG_INVD && REG_LOW3(rm_op->base) == 0b100) ||
            (rm_op->base == REG_INVD)) {
            rm = 0b100;
        } else {
            rm = REG_LOW3(rm_op->base);
        }
    }

    /*
        MODRM.reg encodes either the digit or the register used by
        the instruction. If digit is set to -1 by the instruction
        table, fill MODRM.reg as a register. Otherwise, use digit.
    */
    if (instruction->form->digit == -1) {
        reg = REG_LOW3(reg_op->reg);
    } else {
        reg = instruction->form->digit;
        assert(
            reg < 8 && "Invalid MODRM.reg set, digit exceeds maximum allowed 7."
        );
    }

    modrm |= (mod & 0x03) << 6;
    modrm |= (reg & 0x07) << 3;
    modrm |= (rm & 0x07);
    instruction->binary[instruction->binary_index++] = modrm;
}
