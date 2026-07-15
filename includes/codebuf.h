#ifndef CASMJIT_CODEBUF_H
#define CASMJIT_CODEBUF_H

#include <stddef.h>

#define CB_GROW_FACTOR 2
#define CB_DEFAULT_CAP 4096

typedef struct {
    size_t cap;
    size_t size;
    const char* data;
} codebuf_t;

void cb_reserve(codebuf_t* codebuf, size_t cap);
void cb_free(codebuf_t* codebuf);
void cb_grow(codebuf_t* codebuf);
void cb_add(codebuf_t* codebuf, const void* data, size_t size);

#define CB_CREATE(name)                                                        \
    codebuf_t #name = { 0 };                                                   \
    cb_reserve(&#name, CB_DEFAULT_CAP);

#endif // CASMJIT_CODEBUF_H
