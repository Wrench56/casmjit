#ifndef CASMJIT_X86_INSTR_DEBUG_H
#define CASMJIT_X86_INSTR_DEBUG_H

#include <stdio.h>

#include "jit.h"

#if defined(X86)
#include "x86/instr.h"
#else
#error "Unsupported platform!"
#endif

typedef const char* (*dbg_reg_namer_t)(genreg_t reg);
extern dbg_reg_namer_t casmjit_dbg_reg_namer;

void instr_fdump(FILE* out, const instr_t* instr);
void instr_dump(const instr_t* instr);

void jitcode_fdump(FILE* out, const jitcode_t* jitcode);
void jitcode_dump(const jitcode_t* jitcode);

int operand_snprint(char* buf, size_t n, const operand_t* op);

#ifndef NDEBUG
#define INSTR_DUMP(instr) instr_dump(instr)
#define INSTR_FDUMP(file, instr) instr_fdump(file, instr)
#define JITCODE_DUMP(jitcode) jitcode_dump(jitcode)
#define JITCODE_FDUMP(file, jitcode) jitcode_fdump(file, jitcode)
#else
#define INSTR_DUMP(instr)
#define INSTR_FDUMP(file, instr)
#define JITCODE_DUMP(jitcode)
#define JITCODE_FDUMP(file, jitcode)
#endif // NDEBUG

#endif // CASMJIT_X86_INSTR_DEBUG_H
