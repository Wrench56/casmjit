#include "kritic.h"

#include <stdio.h>

#include "debug.h"
#include "jit.h"

static __attribute__((unused)) void debug(const instr_t* instr) {
    printf("Debug = ");
    for (size_t i = 0; i < instr->binary_index; i++) {
        printf("%02X", instr->binary[i]);
    }
    printf("\n");
    printf("Index = %u\n", instr->binary_index);
}

KRITIC_TEST(jit, emit) {
    jitcode_t code = { 0 };
    jitcode_init(&code);
    labelid_t label = jitcode_emit(
        &code,
        PSEUDO_LABEL,
        (operand_t[]) { { .kind = OPK_PSEUDO, .pseudo_data = "start" },
                        { .kind = OPK_NULL },
                        { .kind = OPK_NULL },
                        { .kind = OPK_NULL } }
    );
    jitcode_emit(
        &code,
        MOV,
        (operand_t[]) {
            { .kind = OPK_REG, .reg = REG_RAX },
            { .kind = OPK_REG, .reg = REG_RBX },
            { .kind = OPK_NULL },
            { .kind = OPK_NULL },
        }
    );
    jitcode_emit(
        &code,
        JMP,
        (operand_t[]) { { .kind = OPK_LABEL, .label = label },
                        { .kind = OPK_NULL },
                        { .kind = OPK_NULL },
                        { .kind = OPK_NULL } }
    );
    jitcode_assemble(&code);
    JITCODE_DUMP(&code);

    jitcode_free(&code);
}
