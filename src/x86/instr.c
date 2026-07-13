#include <assert.h>

#include "x86/encode/disp.h"
#include "x86/encode/form.h"
#include "x86/encode/imm.h"
#include "x86/encode/modrm.h"
#include "x86/encode/opcode.h"
#include "x86/encode/prefix.h"
#include "x86/encode/sib.h"

void to_instr(
    instrkind_t instrkind,
    const operand_t operands[INSTR_MAX_OPERANDS],
    instr_t* instr
) {
    instr->kind = instrkind;
    instr->dispsize = DISP0;
    instr->form = find_instr_form(instrkind, operands);
    instr->binary_index = 0;
    __builtin_memset(instr->binary, 0, sizeof(instr->binary));
    __builtin_memcpy(instr->operands, operands, sizeof(instr->operands));

    assert(instr->form != NULL && "No matching instruction form found!");

    encode_prefix(instr);
    encode_opcode(instr);
    encode_modrm(instr);
    encode_sib(instr);
    encode_disp(instr);
    encode_imm(instr);
}
