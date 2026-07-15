#ifndef CASMJIT_X86_ENCODE_REL_H
#define CASMJIT_X86_ENCODE_REL_H

#include <stdint.h>

#include "x86/instr.h"

void encode_rel(instr_t* instruction);
void encode_pass2_rel(instr_t* instruction, int32_t offset);

#endif // CASMJIT_X86_ENCODE_REL_H
