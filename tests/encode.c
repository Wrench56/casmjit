#include "kritic.h"

#include <stdint.h>
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
        0x48, 0x05, 0xAF, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(ADD, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, i32) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_IMM,
            .immediate = 0xAF,
        },
        {
            .kind = OPK_NULL,
        },
    };

    const uint8_t out[] = {
        0x40, 0x68, 0xAF, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(PUSH, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_rr64) {
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
        0x48, 0x39, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_mi8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RBX,
        },
        {
            .kind = OPK_IMM,
            .immediate = 0x0,
        },
    };

    const uint8_t out[] = {
        0x48, 0x83, 0xFB, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_mi32) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RBX,
        },
        {
            .kind = OPK_IMM,
            .immediate = 0x4D2,
        },
    };

    const uint8_t out[] = {
        0x48, 0x81, 0xFB, 0xD2, 0x04, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_acc32) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_IMM,
            .immediate = 0x4D2,
        },
    };

    const uint8_t out[] = {
        0x48, 0x3D, 0xD2, 0x04, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_rr8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_AL },
        { .kind = OPK_REG, .reg = REG_BL },
    };
    const uint8_t out[] = { 0x40, 0x38, 0xD8 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 3, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_rr16) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_AX },
        { .kind = OPK_REG, .reg = REG_BX },
    };
    const uint8_t out[] = { 0x66, 0x40, 0x39, 0xD8 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 4, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_rr32) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_EAX },
        { .kind = OPK_REG, .reg = REG_EBX },
    };
    const uint8_t out[] = { 0x40, 0x39, 0xD8 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 3, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_mi8_8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_BL },
        { .kind = OPK_IMM, .immediate = 5 },
    };
    const uint8_t out[] = { 0x40, 0x80, 0xFB, 0x05 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 4, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_mi16_8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_BX },
        { .kind = OPK_IMM, .immediate = 5 },
    };
    const uint8_t out[] = { 0x66, 0x40, 0x83, 0xFB, 0x05 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 5, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_mi16_16) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_BX },
        { .kind = OPK_IMM, .immediate = 0x4D2 },
    };
    const uint8_t out[] = { 0x66, 0x40, 0x81, 0xFB, 0xD2, 0x04 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 6, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_mi32_8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_REG, .reg = REG_EBX },
        { .kind = OPK_IMM, .immediate = 5 },
    };
    const uint8_t out[] = { 0x40, 0x83, 0xFB, 0x05 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 4, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_acc8) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_IMM, .immediate = 5 },
        { .kind = OPK_NULL },
    };
    const uint8_t out[] = { 0x40, 0x3C, 0x05 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 3, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(encode, cmp_acc16) {
    instr_t instr = { 0 };
    operand_t operands[4] = {
        { .kind = OPK_IMM, .immediate = 0x4D2 },
        { .kind = OPK_NULL },
    };
    const uint8_t out[] = { 0x66, 0x40, 0x3D, 0xD2, 0x04 };
    to_instr(CMP, operands, &instr);
    KRITIC_ASSERT_EQ((uint32_t) 5, (uint32_t) instr.binary_index);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}
