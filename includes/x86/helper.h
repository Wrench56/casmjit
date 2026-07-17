#ifndef CASMJIT_X86_HELPER_H
#define CASMJIT_X86_HELPER_H

#include "jit.h"
#include "label.h"
#include "x86/regs.h"

static jitcode_t* casmjit_internal_helper_jitcode_ptr;

#define INSTR_IMPL(form, op) INSTR_IMPL_(form, op)
#define INSTR_IMPL_(form, op) INSTRF_##form(op)

#define INSTRF_ZO(op)                                                          \
    static inline void j##op(void) {                                           \
        jitcode_emit(                                                          \
            casmjit_internal_helper_jitcode_ptr,                               \
            op,                                                                \
            (operand_t[4]) { 0 }                                               \
        );                                                                     \
    }

#define INSTRF_M(op)                                                           \
    static inline void j##op(operand_t a0) {                                   \
        jitcode_emit(                                                          \
            casmjit_internal_helper_jitcode_ptr,                               \
            op,                                                                \
            (operand_t[4]) { [0] = a0 }                                        \
        );                                                                     \
    }

#define INSTRF_RM(op)                                                          \
    static inline void j##op(operand_t a0, operand_t a1) {                     \
        jitcode_emit(                                                          \
            casmjit_internal_helper_jitcode_ptr,                               \
            op,                                                                \
            (operand_t[4]) { [0] = a0, [1] = a1 }                              \
        );                                                                     \
    }

#define INSTRF_MR(op) INSTRF_RM(op)
#define INSTRF_MI(op) INSTRF_RM(op)
#define INSTRF_O(op) INSTRF_M(op)
#define INSTRF_D(op) INSTRF_M(op)

#define INSTR_O(op, form, s0, s1, s2, s3, map, pp, mod, opcode, dig)           \
    INSTR_IMPL(form, op)

#include "x86/tbl/instructions.h"

#undef INSTR_O
#undef INSTR_F

#define IMM(value)                                                             \
    (operand_t) {                                                              \
        .kind = OPK_IMM, .immediate = value                                    \
    }
#define REG(reg_)                                                              \
    (operand_t) {                                                              \
        .kind = OPK_REG, .reg = reg_                                           \
    }
#define MEM(base_, index_, scale_, disp_)                                      \
    (operand_t) {                                                              \
        .kind = OPK_MEM, .base = base_, .index = index_, .scale = scale_,      \
        .disp = disp_                                                          \
    }

#define RAX REG(REG_RAX)
#define RCX REG(REG_RCX)
#define RDX REG(REG_RDX)
#define RBX REG(REG_RBX)
#define RSP REG(REG_RSP)
#define RBP REG(REG_RBP)
#define RSI REG(REG_RSI)
#define RDI REG(REG_RDI)

#define R8 REG(REG_R8)
#define R9 REG(REG_R9)
#define R10 REG(REG_R10)
#define R11 REG(REG_R11)
#define R12 REG(REG_R12)
#define R13 REG(REG_R13)
#define R14 REG(REG_R14)
#define R15 REG(REG_R15)

#define INITCODE(name)                                                         \
    jitcode_t name = { 0 };                                                    \
    casmjit_internal_helper_jitcode_ptr = &name;                               \
    jitcode_init(casmjit_internal_helper_jitcode_ptr);
#define FINICODE() jitcode_assemble(casmjit_internal_helper_jitcode_ptr);
#define FREECODE(name) jitcode_free(&name);
#define RUNCODE(name) jitcode_execute(&name);

#endif // CASMJIT_X86_HELPER_H
