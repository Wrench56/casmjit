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

const operand_t m_ops[4] = {
    {
        .kind = OPK_MEM,
        .base = REG_RAX,
        .index = REG_INVD,
        .scale = 1,
    },
    {
        .kind = OPK_NULL,
    },
};

const operand_t r_ops[4] = {
    {
        .kind = OPK_REG,
        .reg = REG_RAX,
    },
    {
        .kind = OPK_NULL,
    },
};

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

const operand_t i8_ops[4] = {
    {
        .kind = OPK_IMM,
        .immediate = 0x10,
    },
    {
        .kind = OPK_NULL,
    }
};

const operand_t i32_ops[4] = {
    {
        .kind = OPK_IMM,
        .immediate = 0xAF,
    },
    {
        .kind = OPK_NULL,
    }
};

const operand_t zo_ops[4] = {
    {
        .kind = OPK_NULL
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

KRITIC_TEST(instructions, xor) {
    instr_t instr = { 0 };

    const uint8_t out_rr[] = {
        0x48, 0x31, 0xD8, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_rm[] = {
        0x48, 0x33, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_ri32[] = {
        0x48, 0x81, 0xF0, 0xAF, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(XOR, rr_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_rr, instr.binary, instr.binary_index));

    to_instr(XOR, rm_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_rm, instr.binary, instr.binary_index));

    to_instr(XOR, ri32_ops, &instr);
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

KRITIC_TEST(instructions, push) {
    instr_t instr = { 0 };

    const uint8_t out_r[] = {
        0x48, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_i8[] = {
        0x40, 0x6A, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_i32[] = {
        0x40, 0x68, 0xAF, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(PUSH, r_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_r, instr.binary, instr.binary_index));

    to_instr(PUSH, i8_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_i8, instr.binary, instr.binary_index));

    to_instr(PUSH, i32_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_i32, instr.binary, instr.binary_index));
}

KRITIC_TEST(instructions, pop) {
    instr_t instr = { 0 };

    const uint8_t out_m[] = {
        0x48, 0x8F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    const uint8_t out_r[] = {
        0x48, 0x58, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(POP, m_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_m, instr.binary, instr.binary_index));

    to_instr(POP, r_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out_r, instr.binary, instr.binary_index));
}

KRITIC_TEST(instructions, syscall) {
    instr_t instr = { 0 };

    const uint8_t out[] = {
        0x0F, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    to_instr(SYSCALL, zo_ops, &instr);
    KRITIC_ASSERT_EQ(0, memcmp(out, instr.binary, instr.binary_index));
}
