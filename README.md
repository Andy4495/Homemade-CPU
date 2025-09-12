# Homemade CPU

Designing my own CPU.

The basic idea is to keep the core design as simple as possible: minimal instruction set, limited internal registers, limited addressing modes. Each instruction can execute in one clock cycle (plus a clock each for fetch and decode).

Use of microcode is still TBD.

Then, create an "intermediate" assembler that supports additional, more complex instructions that are built on the base instructions.

## Initial Design Goals

- 8-bit data bus with 16-bit address bus
- Simple design with minimal instructions
  - Each instruction should execute in one clock cycle (fetch and decode may be additional cycle each)
- Pipeline fetch, decode, execute
- Create an assembler
- Create a compiler of sorts to make a more complete instruction set based on the simple instructions
  - 16-bit equivalents of base instruction set
- No interrupt support
- Possibly have separate logic for imcrementing the PROGRAM COUNTER instead of updating it through the ALU
- Build the CPU in:
  - Hardware (7400-series logic)
    - Mostly CMOS, but may use LS logic for ALU
    - Eventually replace ALU chip with discrete logic
  - Emulator
  - VHDL
 
## Registers

A - Accumulator
B - Accumulator (is this needed?)
  - Actual implementation may be an internal temporary register not available to the programmer
F - Flags (ZERO, CARRY, SIGN, OVERFLOW)
  - The Flags register is updated by every instruction except:
    - NOP, JUMP, CALL, RETURN
PC - Program Counter
SP - Stack Pointer

## Addressing Modes

To be refined (not all may be supported by basic CPU).

Register (operand is in a CPU register)
Immediate (operand is included in the instruction itself)
Direct (instruction contains the full address of operand)
Register Indirect (register specified by instruction contains address of operand)
Indexed (instruction uses a dedicated index register plus a displacement specified in the instruction for address of operand)
Indirect (operand is implicit in the instruction)
Bit (manipulate a specific bit within memory or a register)
 
## Basic Instructions

- NOP
- LOAD
- STORE
- COMPARE (set status bits, but do not save result)
- ADD (8-bit)
- SUBTRACT (8-bit)
- SHIFT (RIGHT, LEFT, ARITHMETIC)
- AND
- OR
- NOT
- XOR
- NAND
- NOR
- XNOR
- JUMP
- CONDITIONAL JUMP (one for each of the Flag register bit states)

### Basic instructions that may end up being implemented in intermediate level

- ROTATE
- INCREMENT (this is the same as "ADD 1")
- DECREMENT (this is the same as "SUBTRACT 1")

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
