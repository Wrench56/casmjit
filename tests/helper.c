#include "kritic.h"

#include <stdio.h>

#include "debug.h"
#include "x86/helper.h"

KRITIC_TEST(helper, x86_syscall) {
    const uint8_t out[] = {
        0x48, 0xC7, 0xC0, 0xE7, 0x00, 0x00, 0x00, 0x48,
        0xC7, 0xC7, 0x05, 0x00, 0x00, 0x00, 0x0F, 0x05
    };

    INITCODE(code);

    jMOV(RAX, IMM(231));
    jMOV(RDI, IMM(5));
    jSYSCALL();

    FINICODE();
    JITCODE_DUMP(&code);

    KRITIC_ASSERT_EQ(0, memcmp(code.codepages, out, code.codesize));

    FREECODE(code);
}
