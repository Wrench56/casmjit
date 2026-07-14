#ifndef INSTR_O
#define INSTR_O(op, form, size0, size1, size2, size3, map, pp, opcode, dig)
#endif
#ifndef INSTR_F
#define INSTR_F(op, form, size0, size1, size2, size3, map, pp, opcode, dig)
#endif

INSTR_O(ADD, MR, 64, 64, -1, -1, MAP_LEGACY, PP_NONE, 0x01, -1)
INSTR_F(ADD, RM, 64, 64, -1, -1, MAP_LEGACY, PP_NONE, 0x03, -1)
INSTR_F(ADD, MI, 64, 8, -1, -1, MAP_LEGACY, PP_NONE, 0x83, 0)
INSTR_F(ADD, MI, 64, 32, -1, -1, MAP_LEGACY, PP_NONE, 0x81, 0)

INSTR_O(MOV, MR, 64, 64, -1, -1, MAP_LEGACY, PP_NONE, 0x89, -1)
INSTR_F(MOV, RM, 64, 64, -1, -1, MAP_LEGACY, PP_NONE, 0x8B, -1)
// FIXME: Support OI form.
INSTR_F(MOV, MI, 64, 32, -1, -1, MAP_LEGACY, PP_NONE, 0xC7, 0)

INSTR_O(PAUSE, ZO, -1, -1, -1, -1, MAP_LEGACY, PP_F3, 0x90, -1)
