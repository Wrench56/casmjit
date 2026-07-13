#ifndef CASMJIT_X86_ENCODE_FORM_H
#define CASMJIT_X86_ENCODE_FORM_H

#include <stddef.h>

#include "x86/instr.h"

extern const instrform_t forms[];
extern const size_t forms_count;

const instrform_t* find_instr_form(
    instrkind_t instruction,
    const operand_t operands[INSTR_MAX_OPERANDS]
);

#endif // CASMJIT_X86_ENCODE_FORM_H
