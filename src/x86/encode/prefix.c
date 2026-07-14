#include <stddef.h>

#include "x86/instr.h"

static uint8_t rex_prefix(instr_t* instruction) {
    uint8_t rex = 0b01000000;

    const operand_t* reg = NULL;
    const operand_t* rm = NULL;
    const operand_t* imm = NULL;

    switch (instruction->form->form) {
        case RM:
            reg = &instruction->operands[0];
            rm = &instruction->operands[1];
            break;
        case MR:
            rm = &instruction->operands[0];
            reg = &instruction->operands[1];
            break;
        case MI:
            rm = &instruction->operands[0];
            imm = &instruction->operands[1];
            break;
        case M:
            rm = &instruction->operands[0];
            break;
        case O:
            reg = &instruction->operands[0];
            break;
        case ZO:
            return 0x00;
        default:
            break;
    }

    /* 
        REX is built up in the following way:
        First half of the byte is a standard magic, 0b0100
        The second is made up from 4 bits in order: WBXR
        W is set if you have any 64 bit GP register use.
        The other 3 are all connected to the use of special
        R8-R15 GP registers. If B is set, you enable the general
        use of R8-R15. If X is set, you enable R8-R15 for use
        of x86 special memory index: [base + index * scale + constant].
        If R is set, you enable the use of R8-R15 for the base.

        Dude, x86 is... complicated.
    */

    /* REX.W */
    if (instruction->form->s0 == 64) {
        rex |= 0b00001000;
    }

    /* REX.R */
    if (reg) {
        if (reg->kind == OPK_REG && REG_EXT(reg->reg)) {
            if (instruction->form->form == O) {
                /* O folds reg into opcode (set R) */
                rex |= 0b00000001;
            } else {
                rex |= 0b00000100;
            }
        }
    }

    /* REX.X and REX.B */
    if (rm) {
        if (rm->kind == OPK_REG) {
            if (REG_EXT(rm->reg)) {
                rex |= 0b00000001;
            }
        } else if (rm->kind == OPK_MEM) {
            if (rm->base != REG_INVD && REG_EXT(rm->base)) {
                rex |= 0b00000001;
            }
            if (rm->index != REG_INVD && REG_EXT(rm->index)) {
                rex |= 0b00000010;
            }
        }
    }

    return rex;
}

void encode_prefix(instr_t* instruction) {
    uint8_t rex = rex_prefix(instruction);

    const uint8_t pp = instruction->form->pp;
    if (pp != PP_NONE) {
        instruction->binary[instruction->binary_index++] = pp;
    }
    
    if (rex != 0x00) {
        instruction->binary[instruction->binary_index++] = rex;
    }
}
