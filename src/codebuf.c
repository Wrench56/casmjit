#include <stdlib.h>

#include "codebuf.h"

void cb_reserve(codebuf_t* codebuf, size_t cap) {
    codebuf->data = malloc(cap);
    codebuf->cap = cap;
    codebuf->size = 0;
}

void cb_grow(codebuf_t* codebuf) {
    const size_t newcap = codebuf->cap * CB_GROW_FACTOR;
    void* newdata = realloc((void*) codebuf->data, newcap);
    codebuf->data = newdata;
    codebuf->cap = newcap;
}

void cb_add(codebuf_t* codebuf, const void* data, size_t size) {
    while (codebuf->size + size > codebuf->cap) {
        cb_grow(codebuf);
    }
    __builtin_memcpy((void*) codebuf->data + codebuf->size, data, size);
    codebuf->size += size;
}
