#if defined(__linux__)

#include <assert.h>
#include <stdio.h>
#include <sys/mman.h>

#include "xmem.h"

void inj_code(jitcode_t* jitcode) {
    void* codepages = mmap(
        NULL,
        jitcode->codesize,
        PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );
    assert(codepages != MAP_FAILED && "mmap() returned NULL!");

    size_t index = 0;
    size_t size = 0;
    instr_t* instruction = &jitcode->codebuf.data[index++];
    while (instruction->kind != PSEUDO_NULL) {
        __builtin_memcpy(
            codepages + size,
            instruction->binary,
            instruction->binary_index
        );

        size += instruction->binary_index;
        instruction = &jitcode->codebuf.data[index++];
    }

    mprotect(codepages, jitcode->codesize, PROT_READ | PROT_EXEC);
    jitcode->codepages = codepages;
}

void free_code(jitcode_t* jitcode) {
    munmap(jitcode->codepages, jitcode->codesize);
}

#endif // __linux__
