# C ASM JIT

Since [AsmJit](https://asmjit.com/) does not have a C FFI (why?) and I needed a JIT, I wrote one. I do not support every edge cases and I focused primarily on x86 long mode because that is what I needed. Feel free to fork and modify to your needs. I am planning to use this project in the future and incrementally expand it, so any raise an issue for any feature you may want. If I have time, chances are I will implement it.

## Developer Notes

### Instruction Table (x86)

#### Macros

The order and syntax for defining instructions is somewhat non-trivial. First off, we have two macros, `INSTR_O` and `INSTR_F`. An `INSTR_O` should only be seen once per instruction, as it is used to create unique enum constants for instructions. To define more forms per instruction, use `INSTR_F` after the first `INSTR_O`.

#### Ordering

By convention, the ordering goes `MR` -> `RM` -> `MI` -> `D` -> `M` -> `O` ->`I` -> `ZO`. As seen below, smaller size constrained instructions should come before bigger ones (e.g. `imm8` before `imm32`) so that the assembler prioritizes them and does not waste space.

```c
INSTR_O(ADD, MR, 64, 64, -1, -1, MAP_LEGACY, PP_NONE, ENCM_NONE, 0x01, -1)
INSTR_F(ADD, RM, 64, 64, -1, -1, MAP_LEGACY, PP_NONE, ENCM_NONE, 0x03, -1)
/* Comes before BOTH imm32 forms */
INSTR_F(ADD, MI, 64, 8, -1, -1, MAP_LEGACY, PP_NONE, ENCM_NONE, 0x83, 0)
INSTR_F(ADD, MI, 64, 32, -1, -1, MAP_LEGACY, PP_NONE, ENCM_ACC_OPCODE, 0x05, -1)
INSTR_F(ADD, MI, 64, 32, -1, -1, MAP_LEGACY, PP_NONE, ENCM_NONE, 0x81, 0)
```
