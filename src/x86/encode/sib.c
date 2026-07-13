#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "x86/instr.h"

void encode_sib(instr_t* instruction) {
    uint8_t sib = 0;
    uint8_t scale;
    uint8_t index;
    uint8_t base;

    const operand_t* rm_op;
    switch (instruction->form->form) {
        case RM:
            rm_op = &instruction->operands[1];
            break;
        case MR:
            rm_op = &instruction->operands[0];
            break;
        case MI:
            rm_op = &instruction->operands[0];
            break;
        case M:
            rm_op = &instruction->operands[0];
            break;
        case I:
        case O:
        default:
            return;
    }

    /* Check if SIB byte is needed in the first place */
    if (rm_op->kind != OPK_MEM) {
        return;
    }

    if (!((rm_op->index != REG_INVD) ||
          (rm_op->base != REG_INVD && REG_LOW3(rm_op->base) == 0b100) ||
          (rm_op->base == REG_INVD))) {
        return;
    }

    assert(
        (rm_op->scale > 0 && rm_op->scale < 9) &&
        "SIB.scale is not within bounds (0 < scale < 9)"
    );
    scale = __builtin_ctz(rm_op->scale);

    if (rm_op->index != REG_INVD) {
        if (rm_op->index == REG_RSP) {
            assert(0 && "RSP Cannot be a SIB.index");
            exit(2);
        }
        index = REG_LOW3(rm_op->index);
    } else {
        index = 0b100;
    }

    if (rm_op->base != REG_INVD) {
        base = REG_LOW3(rm_op->base);
    } else {
        base = 0b101;
    }

    sib |= (scale & 0x03) << 6;
    sib |= (index & 0x07) << 3;
    sib |= (base & 0x07);
    instruction->binary[instruction->binary_index++] = sib;
}
