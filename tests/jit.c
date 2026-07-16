#include "kritic.h"

#include <stdio.h>

#include "debug.h"
#include "jit.h"

KRITIC_TEST(jit, x86_syscall) {
    jitcode_t code = { 0 };
    jitcode_init(&code);

    const uint8_t out[] = {
        0x48, 0xC7, 0xC0, 0xE7, 0x00, 0x00, 0x00, 0x48,
        0xC7, 0xC7, 0x05, 0x00, 0x00, 0x00, 0x0F, 0x05
    };

    labelid_t label = jitcode_emit(
        &code,
        PSEUDO_LABEL,
        (operand_t[]) {
            { .kind = OPK_PSEUDO, .pseudo_data = "start" },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL }
        }
    );
    jitcode_emit(
        &code,
        MOV,
        (operand_t[]) {
            { .kind = OPK_REG, .reg = REG_RAX },
            { .kind = OPK_IMM, .immediate = 231 },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL },
        }
    );
    jitcode_emit(
        &code,
        MOV,
        (operand_t[]) {
            { .kind = OPK_REG, .reg = REG_RDI },
            { .kind = OPK_IMM, .immediate = 5 },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL },
        }
    );
    jitcode_emit(
        &code,
        SYSCALL,
        (operand_t[]) {
            { .kind = OPK_NULL },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL }
        }
    );

    jitcode_assemble(&code);
    JITCODE_DUMP(&code);

    KRITIC_ASSERT_EQ(0, memcmp(code.codepages, out, code.codesize));

    jitcode_free(&code);
}
