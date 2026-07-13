#ifndef INSTR_O
#define INSTR_O(op, form, size0, size1, size2, size3, map, opcode, dig)
#endif
#ifndef INSTR_F
#define INSTR_F(op, form, size0, size1, size2, size3, map, opcode, dig)
#endif

INSTR_O(ADD, MR, 64, 64, -1, -1, MAP_LEGACY, 0x01, -1)
INSTR_F(ADD, RM, 64, 64, -1, -1, MAP_LEGACY, 0x03, -1)
INSTR_F(ADD, MI, 64, 32, -1, -1, MAP_LEGACY, 0x81, 0)
INSTR_F(ADD, MI, 64, 8, -1, -1, MAP_LEGACY, 0x83, 0)
