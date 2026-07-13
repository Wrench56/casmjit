#include "x86/instr.h"

void encode_disp(instr_t* instruction) {
    const instrform_t* form = instruction->form;

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

    switch (instruction->dispsize) {
        case DISP0:
            break;
        case DISP8:
            instruction->binary[instruction->binary_index++] = (int8_t)
                                                                   rm_op->disp;
            break;
        case DISP32:
            __builtin_memcpy(
                instruction->binary + instruction->binary_index,
                &rm_op->disp,
                4
            );
            instruction->binary_index += 4;
            break;
    }
}
