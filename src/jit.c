#include "jit.h"

#include "x86/encode/rel.h"

void jitcode_init(jitcode_t* jitcode) {
    cb_reserve(&jitcode->codebuf, CB_DEFAULT_CAP * sizeof(instr_t));
}

void jitcode_free(jitcode_t* jitcode) {
    cb_free(&jitcode->codebuf);
}

labelid_t jitcode_emit(
    jitcode_t* prog,
    instrkind_t kind,
    const operand_t ops[INSTR_MAX_OPERANDS]
) {
    instr_t instruction = { 0 };
    labelid_t lid = 0;

    if (kind == PSEUDO_LABEL) {
        labelid_t label = create_label();
        instruction.kind = PSEUDO_LABEL;
        instruction.label = label;
        lid = label;

        if (ops != NULL && ops[0].kind == OPK_PSEUDO) {
            instruction.name = ops[0].pseudo_data;
        }
    } else {
        to_instr(kind, ops, &instruction);
    }

    cb_add(&prog->codebuf, &instruction, sizeof(instr_t));
    return lid;
}

static int32_t resolve_offset(
    const instr_t* instrs,
    size_t index_instr,
    size_t index_label
) {
    int32_t bytes = 0;

    size_t start;
    size_t stop;
    if (index_instr < index_label) {
        start = index_instr;
        stop = index_label;
    } else {
        start = index_label;
        stop = index_instr;
    }

    for (size_t i = start; i <= stop; i++) {
        bytes += instrs[i].binary_index;
    }

    return (index_instr < index_label) ? bytes : -bytes;
}

void jitcode_assemble(jitcode_t* jitcode) {
    instr_t nullinstr = { 0 };
    cb_add(&jitcode->codebuf, &nullinstr, sizeof(instr_t));

    size_t index = 0;
    instr_t* instruction = &jitcode->codebuf.data[index++];
    while (instruction->kind != PSEUDO_NULL) {
        if (instruction->kind == JMP) {
            int32_t offset = resolve_offset(
                jitcode->codebuf.data,
                index,
                get_label_index(instruction->label)
            );

            encode_pass2_rel(instruction, offset);
        }
        instruction = &jitcode->codebuf.data[index++];
    }
}
