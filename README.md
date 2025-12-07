# Homemade CPU

[![Check Links](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-links.yml/badge.svg)](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-links.yml)
[![Check Wiki Links](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-wiki-links.yml/badge.svg)](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-wiki-links.yml)

Designing my own CPU:

- [Blog][3] documenting my progress
- [Wiki][2] for design details and documentation
- [Emulator][1] to run the CPU instructions and compare with the actual hardware

## Latest Updates

## 2025-Dec-06: Instruction Set Updates

I made some updates to the instruction set and hardware design:

- Moved the `EXCH` instruction from the core CPU implementation to the assembler
  - That instruction in particular required too many connections to and from `AB` and `AA`
- Removed the hardcoded load instructions (`LDA1`, `LDAF`, `LDB1`, `LDBF`)
  - These provided minimal benefit (1 clock cycle) compared to the additonal hardware needed
- Moved the bit set and clear instructions for `AA` (`BCAA`, `BSAA`) to be implemented by the assembler
- Created another register (`AC`) which can be used as a counter register for loops and for temporary storage. It is not connected to the ALU or `AB`, so minimal additional hardware is needed.
  - Relevant instructions were added to support the new register
  
[(full blog entry...)][ref]

[ref]: https://github.com/Andy4495/Homemade-CPU/wiki/Blog#progress-blog

## Design Goals

- 8-bit data, 16-bit address bus
- Implement with readily-available, off-the-shelf 74HCxx-series logic chips
- Based on RISC principles
  - Each instruction should execute in one clock cycle (not including fetch)
  - Fixed-length 8-bit instructions
  - Limited instruction set
    - Simple enough to implement and execute in one clock, but powerful enough that more complex operations can easily be implemented by the assembler/compiler
  - No microcode
  - Although RISC CPUs typically have a high number of internal registers, this implementation will have relatively few registers
- Little Endian (least significant byte at lowest address)
- von Neumann architecture
- `PC` and `SP` are implemented with counter chips (74HC161 for `PC`, 74HC193 or 74HC194 for `SP`) for ease of incrementing/decrementing without having to use the ALU

## Longer Term

- Create an assembler
  - The assembler will also implement higher-level operations beyond what is supported by the CPU
- Create a hardware model (VHDL or Verilog)
  - This may lag a little, as my VHDL is a little rusty at this point
- Additional CPU improvements not implemented in the first iteration of the design:
  - Pipeline fetch, decode, execute
  - Interrupt support

## Next Steps

Start implementing the instruction decoding logic.

## History

| Date     | Activity |
| -------- | -------- |
| Sep-2025 | Document project design goals. Set up repo. |
| Oct-2025 | Initial CPU design: registers, addressing modes, opcode size and format, instruction set. |
|          | Create wiki. |
|          | Implement [emulator][1]: disassembler, emulator, automated tests. |
|          | Update design to version 2 to use 8-bit instructions instead of 16-bit. |
|          | - Updated [emulator][1] to support design version 2. |
| Nov-2025 | `PC`, `IR`, and related hardware implemented for instruction fetch, `PC` incrementing |

[1]: https://github.com/Andy4495/emulator-8-bit
[2]: https://github.com/Andy4495/Homemade-CPU/wiki
[3]: https://github.com/Andy4495/Homemade-CPU/wiki/Blog
