#include <assert.h>
#include <stddef.h>

#include "x86/instr.h"

void encode_imm(instr_t* instruction) {
    const instrform_t* form = instruction->form;
    const operand_t* imm_op = NULL;
    switch (form->form) {
        case MI:
            imm_op = &instruction->operands[1];
            break;
        case I:
            imm_op = &instruction->operands[0];
            break;
        default:
            return;
    }

    size_t bytes = ((form->form == I) ? form->s0 : form->s1) / 8;
    assert(bytes < 9 && "Immediate length exceeds 8 bytes!");

    __builtin_memcpy(
        instruction->binary + instruction->binary_index,
        &imm_op->immediate,
        bytes
    );
    instruction->binary_index += bytes;
}
