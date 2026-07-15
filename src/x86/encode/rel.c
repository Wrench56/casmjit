#include "label.h"
#include "x86/instr.h"

void encode_rel(instr_t* instruction) {
    if (instruction->form->form != D) {
        return;
    }

    const size_t label_index = get_label_index(instruction->operands[0].label);
    const size_t this_index = instruction->operands[0].this_index;

    if (instruction->kind == JMP) {
        instruction->binary_index += 4;
    }
}

void encode_pass2_rel(instr_t* instruction, int32_t offset) {
    __builtin_memcpy(
        instruction->binary + 1,
        &offset,
        sizeof(offset)
    );
}
