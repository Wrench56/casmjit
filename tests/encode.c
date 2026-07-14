#include "kritic.h"

#include <stdio.h>

#include "x86/instr.h"

static __attribute__((unused)) void debug(const instr_t* instr) {
    printf("Debug = ");
    for (size_t i = 0; i < instr->binary_index; i++) {
        printf("%02X", instr->binary[i]);
    }
    printf("\n");
    printf("Index = %u\n", instr->binary_index);
}

KRITIC_TEST(encode, rr64) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_REG,
            .reg = REG_RBX,
        },
    };

    const uint8_t out[] = {
        0x48, 0x01, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(ADD, operands, &instr);

    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, rm64) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_MEM,
            .base = REG_RBX,
            .index = REG_INVD,
            .scale = 1,
        },
    };

    const uint8_t out[] = {
        0x48, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(ADD, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

// TODO: Use accumulator form
KRITIC_TEST(encode, r64i8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_IMM,
            .immediate = 0x01,
        },
    };

    const uint8_t out[] = {
        0x48, 0x83, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(ADD, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, r64i32) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_IMM,
            .immediate = 0xAF,
        },
    };

    const uint8_t out[] = {
        0x48, 0x81, 0xC0, 0xAF, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(ADD, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}
