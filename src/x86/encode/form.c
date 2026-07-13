#include <stddef.h>

#include "x86/instr.h"

#define INSTR_O(op, form_, s0_, s1_, s2_, s3_, map_, opcode_, dig)             \
    { .kind = op,                                                              \
      .form = form_,                                                           \
      .s0 = s0_,                                                               \
      .s1 = s1_,                                                               \
      .s2 = s2_,                                                               \
      .s3 = s3_,                                                               \
      .map = map_,                                                             \
      .opcode = opcode_,                                                       \
      .digit = dig },
#define INSTR_F(op, form, s0, s1, s2, s3, map, opcode, dig)                    \
    INSTR_O(op, form, s0, s1, s2, s3, map, opcode, dig)

const instrform_t forms[] = {
#include "x86/tbl/instructions.h"
};
#undef INSTR_O
#undef INSTR_F

const size_t forms_count = (sizeof(forms) / sizeof(forms[0]));

const instrform_t* find_instr_form(
    instrkind_t instruction,
    const operand_t operands[INSTR_MAX_OPERANDS]
) {
    form_t form = UNKNOWN;

    if (operands[0].kind == OPK_MEM) {
        if (operands[1].kind == OPK_REG) {
            form = MR;
        }

        if (operands[1].kind == OPK_IMM) {
            form = MI;
        }

        if (operands[1].kind == OPK_NULL) {
            form = M;
        }
    } else if (operands[0].kind == OPK_REG) {
        if (operands[1].kind == OPK_REG) {
            form = MR;
        } else if (operands[1].kind == OPK_MEM) {
            form = RM;
        } else if (operands[1].kind == OPK_IMM) {
            form = MI;
        }
    } else if (operands[0].kind == OPK_IMM && operands[1].kind == OPK_NULL) {
        form = I;
    } else if (operands[0].kind == OPK_NULL) {
        form = O;
    }

    for (size_t i = 0; i < forms_count; i++) {
        if (forms[i].kind != instruction) {
            continue;
        }

        if (forms[i].form != form) {
            continue;
        }

        if (forms[i].s0 > -1 &&
            REG_SIZE(operands[0].reg) != __builtin_ctz(forms[i].s0) - 3) {
            continue;
        }

        if (forms[i].s1 > -1 &&
            REG_SIZE(operands[1].reg) != __builtin_ctz(forms[i].s1) - 3) {
            continue;
        }

        if (forms[i].s2 > -1 &&
            REG_SIZE(operands[2].reg) != __builtin_ctz(forms[i].s2) - 3) {
            continue;
        }

        if (forms[i].s3 > -1 &&
            REG_SIZE(operands[3].reg) != __builtin_ctz(forms[i].s3) - 3) {
            continue;
        }

        return &forms[i];
    }

    return NULL;
}
