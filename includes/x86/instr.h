#ifndef CASMJIT_X86_INSTR_H
#define CASMJIT_X86_INSTR_H

#include <stddef.h>
#include <stdint.h>

#include "label.h"
#include "x86/regs.h"

#define INSTR_MAX_OPERANDS 4
#define INSTR_MAX_LENGTH 16

typedef enum { UNKNOWN = 0, RM, MR, MI, M, I, O, ZO, D } form_t;

typedef enum {
    OPK_NULL = 0,
    OPK_LABEL = OPKIND_LABEL,
    OPK_PSEUDO = 2,
    OPK_REG,
    OPK_IMM,
    OPK_MEM,
} opkind_t;

typedef enum {
    MAP_LEGACY = 0,
    MAP_0F,
    MAP_0F38,
    MAP_0F3A,
} opmap_t;

typedef enum {
    PP_NONE = 0x00,
    PP_66 = 0x66,
    PP_F3 = 0xF3,
} oppp_t;

typedef enum {
    DISP0,
    DISP8,
    DISP32,
} dispsize_t;

#define INSTR_O(op, form, s0, s1, s2, s3, map, pp, opcode, dig) op,
typedef enum {
    PSEUDO_NULL = 0,
    PSEUDO_LABEL = 1,
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
    opmap_t map;
    oppp_t pp;
    uint8_t opcode;
    int8_t digit;
} instrform_t;

typedef struct {
    opkind_t kind;
    union {
        struct {
            genreg_t reg;
        };
        struct {
            int64_t immediate;
        };
        struct {
            genreg_t base;
            genreg_t index;
            uint8_t scale;
            int32_t disp;
        };
        struct {
            labelid_t label;
            size_t this_index;
        };
        struct {
            void* pseudo_data;
        };
    };
} operand_t;

typedef struct {
    instrkind_t kind;
    union {
        struct {
            dispsize_t dispsize;
            uint8_t binary_index;
            uint8_t binary[INSTR_MAX_LENGTH];
            const instrform_t* form;
            operand_t operands[INSTR_MAX_OPERANDS];
        };
        struct {
            labelid_t label;
            const char* name;
        };
    };
} instr_t;

void to_instr(
    instrkind_t instrkind,
    const operand_t operands[INSTR_MAX_OPERANDS],
    instr_t* instr
);

#endif // CASMJIT_X86_INSTR_H
