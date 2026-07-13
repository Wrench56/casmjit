#ifndef CASMJIT_X86_INSTR_H
#define CASMJIT_X86_INSTR_H

#include <stdint.h>

#include "regs.h"

#define INSTR_MAX_OPERANDS 4
#define INSTR_MAX_LENGTH 16

typedef enum { UNKNOWN = 0, RM, MR, MI, M, I, O } form_t;

typedef enum {
    OPK_NULL = 0,
    OPK_REG,
    OPK_IMM,
    OPK_MEM,
} opkind_t;

#define INSTR_O(op, form, s0, s1, s2, s3, opcode, dig) op,
typedef enum {
#include "x86/tbl/instructions.h"
} instrkind_t;
#undef INSTR_O
#undef INSTR_F

typedef struct {
    instrkind_t kind;
    form_t form;
    int8_t s0;
    int8_t s1;
    int8_t s2;
    int8_t s3;
    uint8_t opcode;
    uint8_t digit;
} instrform_t;

typedef struct {
    opkind_t kind;
    union {
        struct {
            genreg_t reg;
        };
        struct {
            uint64_t immediate;
        };
        struct {
            genreg_t base;
            genreg_t index;
            uint8_t scale;
            int32_t disp;
        };
    };
} operand_t;

typedef struct {
    instrkind_t kind;
    uint8_t binary_index;
    uint8_t binary[INSTR_MAX_LENGTH];
    const instrform_t* form;
    operand_t operands[INSTR_MAX_OPERANDS];
} instr_t;

#endif // CASMJIT_X86_INSTR_H
