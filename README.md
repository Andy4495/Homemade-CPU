# Homemade CPU

Designing my own CPU.

The basic idea is to keep the core design as simple as possible: minimal instruction set, limited internal registers, limited addressing modes. Each instruction can execute in one clock cycle (plus a clock each for fetch and decode).

Then, create an "intermediate" assembler that supports additional, more complex instructions that are built on the base instructions.

## Initial Design Goals

- Based on RISC principles
  - Efficient execution by pipelined processor
  - Each instruction should execute in one clock cycle (fetch and decode may be additional cycle each)
  - Fixed-length instructions with a simple encoding
    - May need to consider a special cases for
      - Long jump to any 16-bit address
      - Support for 16-bit immediate values
- Little Endian (least significant byte at lowest address)
- Use of microcode is TBD.
  - If initial design uses microcode, eventually update design so it isn't needed
- Support for hard-coded constants (0, 1, 255, 16383, 32767, 65535)
- Harvard vs. von Neumann architecture TBD
- Opcode length TBD
- 8-bit data bus with 16-bit address bus
- Simple design with minimal instructions
- Pipeline fetch, decode, execute
- Create an assembler
- Create a compiler of sorts to make a more complete instruction set based on the simple instructions
  - 16-bit equivalents of base instruction set
- Interrupt support may be added later
- Possibly have separate logic for incrementing the PROGRAM COUNTER instead of updating it through the ALU
- Build the CPU in:
  - Hardware (7400-series logic)
    - Mostly CMOS, but may use LS logic for ALU
    - Eventually replace ALU chip with discrete logic
  - Emulator
  - VHDL
- Track development through a blog
  - Document each step in the process: why a design decision was made, why something was later changed
  - Use a tracker table to show progress of my various design goals (implemnted, not implemted yet, will not implement)
- Reset vector
- Interrupt vector (save space for this, even if interrupts are not initially supported)
- Trap vectors (TBD)

## Registers

|    |                 |      |
| -- | --------------- | -----|
|  A | Accumulator     | |
|  B | Accumulator     | Actual implementation may be an internal register not available to the programmer |
|  F | Flags           | ZERO (Z), CARRY (C) , SIGN (S), OVERFLOW (V)  |
| PC | Program Counter | |
| SP | Stack Pointer   | |

## Addressing Modes

To be refined (not all may be supported by basic CPU).

| | |
| - | - |
| Register          | Operand is in a CPU register |
| Immediate         | Operand is included in instruction itself |
| Direct            | Instruction contains the full address of operand |
| Indirect          | Operand is implicit in the instruction |
| Register Indirect | Register specified by instruction contains address of operand |
| Indexed           | Instruction uses a dedicated index register plus a displacement specified in the instruction |
| Bit               | Manipulate a specific bit within memory or a register |

## Basic Instructions

I chose 4-character mnemonics to simplify the assembler implementation.

| Mnemonic | Description                | Flags (ZCSV) | Notes |
| -------- | -------------------------- | ------------ | ----- |
| `NOOP`   | No operation               | ----         |       |
| `LOAD`   | Memory into register       | ----         |       |
| `STOR`   | Register into memory       | ----         |       |
| `COMP`   | Subtract, result discarded | ZCSV         |       |
| `ADDD`   | Add                        | ZCSV         |       |
| `SHRL`   | Shift right logical        | Z000         |       |
| `SHLL`   | Shift left logical         | Z000         |       |
| `SHRA`   | Shift right arithmetic     | Z0S0         |       |
| `SHLA`   | Shift left arithmetic      | Z0S0         | This may be same as SHLL      |
| `ANDD`   | Bitwise AND                | Z000         |       |
| `ORRR`   | Bitwise OR                 | Z000         |       |
| `NOTT`   | Bitwise NOT                | Z000         |       |
| `XORR`   | Bitwise XOR                | Z000         |       |
| `NAND`   | Bitwise NAND               | Z000         |       |
| `NORR`   | Bitwise NOR                | Z000         |       |
| `XNOR`   | Bitwise XNOR               | Z000         |       |
| `JUMP`   | Unconditional jump             | ----     |       |
| `JPZS`   | Jump if Zero flag is set       | ----     |       |
| `JPZC`   | Jump if Zero flag is clear     | ----     |       |
| `JPCS`   | Jump if Carry flag is set      | ----     |       |
| `JPCC`   | Jump if Carry flag is clear    | ----     |       |
| `JPSS`   | Jump if Sign flag is set       | ----     |       |
| `JPSC`   | Jump if Sign flag is clear     | ----     |       |
| `JPVS`   | Jump if Overflow flag is set   | ----     |       |
| `JPVC`   | Jump if Overflow flag is clear | ----     |       |

### Basic instructions that may end up being implemented in intermediate level

- Rotate
- Increment (this is the same as "ADD 1")
- Decrement (this is the same as "SUBTRACT 1")

## Intermediate Instructions

These will be supported through the use of a "next level" assembler.

- ADD (16-bit)
- ADD (32-bit)
- SUBTRACT (16-bit)
- SUBTRACT (32-bit)
- NEGATE (1's complement: 8, 16, 32 bit)
- COMPLEMENT (2's complement: 8, 16, 32 bit)
- CALL
- CONDITIONAL CALL (one for each of the Flag register bit states)
- RETURN
- Individually set/clear bits in FLAGS register
- Add index addressing modes to existing instructions
