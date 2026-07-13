#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "x86/instr.h"

static inline void emit_byte(instr_t* instruction, uint8_t byte) {
    instruction->binary[instruction->binary_index++] = byte;
}

void encode_opcode(instr_t* instruction) {
    const instrform_t* form = instruction->form;
    switch (form->map) {
        case MAP_LEGACY:
            break;
        case MAP_0F:
            emit_byte(instruction, 0x0F);
            break;
        case MAP_0F38:
            emit_byte(instruction, 0x0F);
            emit_byte(instruction, 0x38);
            break;
        case MAP_0F3A:
            emit_byte(instruction, 0x0F);
            emit_byte(instruction, 0x3A);
            break;
    }
    emit_byte(instruction, form->opcode);
}
