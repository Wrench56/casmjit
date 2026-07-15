#ifndef CASMJIT_CODEBUF_H
#define CASMJIT_CODEBUF_H

#include <stddef.h>

#if defined(X86)
#include "x86/instr.h"
#else
#error "Unsupported platform!"
#endif

#define CB_GROW_FACTOR 2
#define CB_DEFAULT_CAP 1024

typedef struct {
    size_t cap;
    size_t size;
    instr_t* data;
} codebuf_t;

void cb_reserve(codebuf_t* codebuf, size_t cap);
void cb_free(codebuf_t* codebuf);
void cb_grow(codebuf_t* codebuf);
void cb_add(codebuf_t* codebuf, const instr_t* data, size_t size);

#define CB_CREATE(name)                                                        \
    codebuf_t #name = { 0 };                                                   \
    cb_reserve(&#name, CB_DEFAULT_CAP * sizeof(instr_t));

#endif // CASMJIT_CODEBUF_H
