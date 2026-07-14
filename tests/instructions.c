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

const operand_t rr_ops[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_REG,
            .reg = REG_RBX,
        }
    };

const operand_t ri32_ops[4] = {
        {
            .kind = OPK_REG,
            .reg = REG_RAX,
        },
        {
            .kind = OPK_IMM,
            .immediate = 0xAF,
        }
    };

const operand_t rm_ops[4] = {
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

KRITIC_TEST(instructions, add) {
    instr_t instr = { 0 };

    const uint8_t out_rr[] = {
        0x48, 0x01, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_rm[] = {
        0x48, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_ri32[] = {
        0x48, 0x81, 0xC0, 0xAF, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(ADD, rr_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_rr, instr.binary, instr.binary_index));

    to_instr(ADD, rm_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_rm, instr.binary, instr.binary_index));

    to_instr(ADD, ri32_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_ri32, instr.binary, instr.binary_index));
}

KRITIC_TEST(instructions, mov) {
    instr_t instr = { 0 };

    const uint8_t out_rr[] = {
        0x48, 0x89, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_rm[] = {
        0x48, 0x8B, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_ri32[] = {
        0x48, 0xC7, 0xC0, 0xAF, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(MOV, rr_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_rr, instr.binary, instr.binary_index));

    to_instr(MOV, rm_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_rm, instr.binary, instr.binary_index));

    to_instr(MOV, ri32_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_ri32, instr.binary, instr.binary_index));
}

KRITIC_TEST(instructions, pause) {
    instr_t instr = { 0 };

    const uint8_t out[] = {
        0xF3, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(PAUSE, NULL, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}

KRITIC_TEST(instructions, nop) {
    instr_t instr = { 0 };

    const uint8_t out[] = {
        0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(NOP, NULL, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}
