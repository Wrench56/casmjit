#ifndef CASMJIT_JIT_H
#define CASMJIT_JIT_H

#include <stddef.h>
#include <stdint.h>

#include "codebuf.h"

#if defined(X86)
#include "x86/instr.h"
#else
#error "Unsupported platform!"
#endif

#define MAX_ARGS 4

typedef struct {
    codebuf_t codebuf;
    size_t codesize;
    void* codepages;
} jitcode_t;

void jitcode_init(jitcode_t* jitcode);
void jitcode_free(jitcode_t* jitcode);
labelid_t jitcode_emit(
    jitcode_t* prog,
    instrkind_t kind,
    const operand_t ops[INSTR_MAX_OPERANDS]
);
void jitcode_assemble(jitcode_t* jitcode);

#endif // CASMJIT_JIT_H
