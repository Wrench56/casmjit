#include "debug.h"

#ifndef NDEBUG

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

static const char* const g_reg_names[256] = {
    [REG_AL] = "al",     [REG_CL] = "cl",     [REG_DL] = "dl",
    [REG_BL] = "bl",     [REG_SPL] = "spl",   [REG_BPL] = "bpl",
    [REG_SIL] = "sil",   [REG_DIL] = "dil",   [REG_AH] = "ah",
    [REG_CH] = "ch",     [REG_DH] = "dh",     [REG_BH] = "bh",
    [REG_R8B] = "r8b",   [REG_R9B] = "r9b",   [REG_R10B] = "r10b",
    [REG_R11B] = "r11b", [REG_R12B] = "r12b", [REG_R13B] = "r13b",
    [REG_R14B] = "r14b", [REG_R15B] = "r15b",

    [REG_AX] = "ax",     [REG_CX] = "cx",     [REG_DX] = "dx",
    [REG_BX] = "bx",     [REG_SP] = "sp",     [REG_BP] = "bp",
    [REG_SI] = "si",     [REG_DI] = "di",     [REG_R8W] = "r8w",
    [REG_R9W] = "r9w",   [REG_R10W] = "r10w", [REG_R11W] = "r11w",
    [REG_R12W] = "r12w", [REG_R13W] = "r13w", [REG_R14W] = "r14w",
    [REG_R15W] = "r15w",

    [REG_EAX] = "eax",   [REG_ECX] = "ecx",   [REG_EDX] = "edx",
    [REG_EBX] = "ebx",   [REG_ESP] = "esp",   [REG_EBP] = "ebp",
    [REG_ESI] = "esi",   [REG_EDI] = "edi",   [REG_R8D] = "r8d",
    [REG_R9D] = "r9d",   [REG_R10D] = "r10d", [REG_R11D] = "r11d",
    [REG_R12D] = "r12d", [REG_R13D] = "r13d", [REG_R14D] = "r14d",
    [REG_R15D] = "r15d",

    [REG_RAX] = "rax",   [REG_RCX] = "rcx",   [REG_RDX] = "rdx",
    [REG_RBX] = "rbx",   [REG_RSP] = "rsp",   [REG_RBP] = "rbp",
    [REG_RSI] = "rsi",   [REG_RDI] = "rdi",   [REG_R8] = "r8",
    [REG_R9] = "r9",     [REG_R10] = "r10",   [REG_R11] = "r11",
    [REG_R12] = "r12",   [REG_R13] = "r13",   [REG_R14] = "r14",
    [REG_R15] = "r15",

    [REG_INVD] = "none",
};

static const char* dbg_reg_name(genreg_t r) {
    const char* s = g_reg_names[(unsigned) r & 0xFF];
    return s;
}

static const char* form_name(form_t f) {
    switch (f) {
        case UNKNOWN:
            return "UNKNOWN";
        case RM:
            return "RM";
        case MR:
            return "MR";
        case MI:
            return "MI";
        case M:
            return "M";
        case I:
            return "I";
        case O:
            return "O";
        case ZO:
            return "ZO";
        case D:
            return "D";
    }
}

static const char* opkind_name(opkind_t k) {
    switch (k) {
        case OPK_NULL:
            return "NULL";
        case OPK_LABEL:
            return "LABEL";
        case OPK_PSEUDO:
            return "PSEUDO";
        case OPK_REG:
            return "REG";
        case OPK_IMM:
            return "IMM";
        case OPK_MEM:
            return "MEM";
    }
}

static const char* opmap_name(opmap_t m) {
    switch (m) {
        case MAP_LEGACY:
            return "legacy";
        case MAP_0F:
            return "0F";
        case MAP_0F38:
            return "0F38";
        case MAP_0F3A:
            return "0F3A";
    }
}

static const char* oppp_name(oppp_t p) {
    switch (p) {
        case PP_NONE:
            return "none";
        case PP_66:
            return "66";
        case PP_F3:
            return "F3";
    }
}

static const char* dispsize_name(dispsize_t d) {
    switch (d) {
        case DISP0:
            return "DISP0";
        case DISP8:
            return "DISP8";
        case DISP32:
            return "DISP32";
    }
}

static const char* const g_instrkind_names[] = {
    [PSEUDO_NULL] = "PSEUDO_NULL",
    [PSEUDO_LABEL] = "PSEUDO_LABEL",
#define INSTR_O(op, form, s0, s1, s2, s3, map, pp, opcode, dig) [op] = #op,
#include "x86/tbl/instructions.h"
#undef INSTR_O
};

static const char* instrkind_name(instrkind_t k) {
    size_t i = (size_t) k;
    if (i < sizeof g_instrkind_names / sizeof *g_instrkind_names &&
        g_instrkind_names[i]) {
        return g_instrkind_names[i];
    }

    return "UNKNOWN";
}

int operand_snprint(char* buf, size_t n, const operand_t* op) {
    switch (op->kind) {
        case OPK_NULL:
            return snprintf(buf, n, "-");
        case OPK_REG:
            return snprintf(buf, n, "%s", dbg_reg_name(op->reg));
        case OPK_IMM:
            return snprintf(
                buf,
                n,
                "0x%" PRIx64 " (%" PRId64 ")",
                (uint64_t) op->immediate,
                op->immediate
            );
        case OPK_MEM:
            return snprintf(
                buf,
                n,
                "[base=%s index=%s scale=%u disp=0x%" PRIx32 " (%" PRId32 ")]",
                dbg_reg_name(op->base),
                dbg_reg_name(op->index),
                (unsigned) op->scale,
                (uint32_t) op->disp,
                op->disp
            );
        case OPK_LABEL:
            return snprintf(
                buf,
                n,
                "label #%jd @%zu",
                (intmax_t) op->label,
                op->this_index
            );
        case OPK_PSEUDO:
            return snprintf(buf, n, "pseudo %p", op->pseudo_data);
    }

    return -1;
}

void instr_fdump(FILE* out, const instr_t* instr) {
    if (!out) {
        out = stdout;
    }
    if (!instr) {
        fputs("instr <null ptr>\n", out);
        return;
    }

    if (instr->kind == PSEUDO_LABEL) {
        fprintf(
            out,
            "LABEL #%jd \"%s\"\n",
            (intmax_t) instr->label,
            instr->name ? instr->name : "(null)"
        );
        return;
    }
    if (instr->kind == PSEUDO_NULL) {
        fputs("<null instr>\n", out);
        return;
    }

    fprintf(
        out,
        "%s  [kind=%d form=%s]\n",
        instrkind_name(instr->kind),
        (int) instr->kind,
        instr->form ? form_name(instr->form->form) : "?"
    );

    if (instr->form) {
        const instrform_t* f = instr->form;
        fprintf(
            out,
            "     enc: map=%s pp=%s opcode=0x%02X ",
            opmap_name(f->map),
            oppp_name(f->pp),
            f->opcode
        );
        if (f->digit >= 0) {
            fprintf(out, "/%d\n", f->digit);
        } else {
            fputs("\n", out);
        }
    } else {
        fputs("     enc: <no form>\n", out);
    }

    fprintf(out, "     dispsize=%s\n", dispsize_name(instr->dispsize));

    fprintf(out, "     bytes[%u]:", (unsigned) instr->binary_index);
    for (unsigned i = 0; i < instr->binary_index && i < INSTR_MAX_LENGTH; i++) {
        fprintf(out, " %02X", instr->binary[i]);
    }
    fputc('\n', out);

    for (int i = 0; i < INSTR_MAX_OPERANDS; i++) {
        const operand_t* op = &instr->operands[i];
        if (op->kind == OPK_NULL) {
            continue;
        }
        char line[96];
        operand_snprint(line, sizeof line, op);
        fprintf(out, "     op[%d] %-6s %s\n", i, opkind_name(op->kind), line);
    }
}

void instr_dump(const instr_t* instr) {
    instr_fdump(stdout, instr);
}

void jitcode_fdump(FILE* out, const jitcode_t* jitcode) {
    size_t inum = jitcode->codebuf.size / sizeof(instr_t);

    for (size_t i = 0; i < inum; i++) {
        instr_t* instr = &jitcode->codebuf.data[i];
        instr_fdump(out, instr);
    }
}

void jitcode_dump(const jitcode_t* jitcode) {
    jitcode_fdump(stdout, jitcode);
}

#else

__attribute__((unused)) int operand_snprint(
    char* buf,
    size_t n,
    const operand_t* op
) {
    (void) buf;
    (void) n;
    (void) op;
    return 0;
}

__attribute__((unused)) void instr_fdump(FILE* out, const instr_t* instr) {
    (void) out;
    (void) instr;
}

__attribute__((unused)) void instr_dump(const instr_t* instr) {
    (void) instr;
}

__attribute__((unused)) void jitcode_fdump(
    FILE* out,
    const jitcode_t* jitcode
) {
    (void) jitcode;
}

#endif // NDEBUG
