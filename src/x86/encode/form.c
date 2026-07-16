#include <stdbool.h>
#include <stddef.h>

#include "x86/instr.h"

#define INSTR_O(op, form_, s0_, s1_, s2_, s3_, map_, pp_, mod_, opcode_, dig)  \
    { .kind = op,                                                              \
      .form = form_,                                                           \
      .s0 = s0_,                                                               \
      .s1 = s1_,                                                               \
      .s2 = s2_,                                                               \
      .s3 = s3_,                                                               \
      .map = map_,                                                             \
      .pp = pp_,                                                               \
      .mods = mod_,                                                            \
      .opcode = opcode_,                                                       \
      .digit = dig },
#define INSTR_F(op, form, s0, s1, s2, s3, map, pp, mod, opcode, dig)           \
    INSTR_O(op, form, s0, s1, s2, s3, map, pp, mod, opcode, dig)

const instrform_t forms[] = {
#include "x86/tbl/instructions.h"
};
#undef INSTR_O
#undef INSTR_F

const size_t forms_count = (sizeof(forms) / sizeof(forms[0]));

static inline bool check_bounds(const int8_t bounds, const operand_t* op) {
    if (bounds == -1) {
        return true;
    }

    switch (op->kind) {
        case OPK_REG:
            return (REG_SIZE(op->reg) == __builtin_ctz(bounds) - 3);
        case OPK_MEM:
            return true;
        case OPK_IMM:;
            if (bounds >= 64) {
                return true;
            }

            int64_t max = (1 << (bounds - 1)) - 1;
            int64_t min = -(1 << (bounds - 1));
            return (op->immediate <= max && op->immediate >= min);
        case OPK_LABEL:
            return (bounds == 32) ? true : false;
        default:
            return false;
    }

    return false;
}

const instrform_t* find_instr_form(
    instrkind_t instruction,
    const operand_t operands[INSTR_MAX_OPERANDS]
) {
    form_t form = UNKNOWN;

    if (operands == NULL || operands[0].kind == OPK_NULL) {
        form = ZO;
    } else if (operands[0].kind == OPK_MEM) {
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
        } else if (operands[1].kind == OPK_NULL) {
            form = O;
        }
    } else if (operands[0].kind == OPK_IMM && operands[1].kind == OPK_NULL) {
        form = I;
    } else if (operands[0].kind == OPK_LABEL && operands[1].kind == OPK_NULL) {
        form = D;
    }

    for (size_t i = 0; i < forms_count; i++) {
        if (forms[i].kind != instruction) {
            continue;
        }

        if (forms[i].form != form) {
            continue;
        }

        /* Accumulator form is only valid in I form with (R)AX  */
        if (form == MI && forms[i].mods & ENCM_ACC_OPCODE &&
            operands[0].reg != REG_RAX) {
            continue;
        }

        if (!check_bounds(forms[i].s0, &operands[0])) {
            continue;
        }

        if (!check_bounds(forms[i].s1, &operands[1])) {
            continue;
        }

        if (!check_bounds(forms[i].s2, &operands[2])) {
            continue;
        }

        if (!check_bounds(forms[i].s3, &operands[3])) {
            continue;
        }

        return &forms[i];
    }

    return NULL;
}
