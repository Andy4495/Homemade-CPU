# Homemade CPU

[![Check Links](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-links.yml/badge.svg)](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-links.yml)
[![Check Wiki Links](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-wiki-links.yml/badge.svg)](https://github.com/Andy4495/Homemade-CPU/actions/workflows/check-wiki-links.yml)

Designing my own CPU.

Check out my [blog][3] to see my progress.

See the [Wiki][2] for design details and documentation.

An [emulator][1] is available to run the CPU instructions and compare with the actual hardware.

## Latest Updates

### 2025-Oct-24: Starting on the Hardware

[ref]: https://github.com/Andy4495/Homemade-CPU/wiki/Blog#2025-oct-24-starting-on-the-hardware

Now that I have a pretty good idea on the design of the CPU, it's time to get started on the hardware. First step is the clock generator circuit. [(full blog entry...)][ref]

## Design Goals

- 8-bit data, 16-bit address bus
- Implement with readily-available, off-the-shelf logic chips
  - 74HCxx-series
- Based on RISC principles
  - Each instruction should execute in one clock cycle (not including fetch)
  - Fixed-length 8-bit instructions
  - Limited instruction set
    - Simple enough to implement and execute in one clock, but powerful enough that more complex operations can easily be implemented by the assembler/compiler
  - No microcode
  - Although RISC CPUs typically have a high number of internal registers, this implementation will have relatively few registers
- Little Endian (least significant byte at lowest address)
- von Neumann architecture
- PC and SP are implemented with a counter chip (e.g., 74HC161 or 74HC163) for ease of incrementing without having to use the ALU
- Additional ideas that may be implemented
  - Pipeline fetch, decode, execute
  - Support for hard-coded constants (0, 1, 255)
  - Interrupt support
- Create and update an assembler as the hardware is implemented
  - The assembler will probably implement higher-level operations beyond what is supported by the CPU
    - For example, 16-bit add
- Create and update an emulator as the hardware is implemented
- Create and update a hardware model (VHDL or Verilog) as the hardware is implemented
  - This may lag a little, as my VHDL is a little rusty at this point
- Track development
  - Document each step in the process: why a design decision was made, why something was later changed
  - Use a tracker table to show progress of my various design goals (implemnted, not implemted yet, will not implement)

## Next Steps

Create an emulator/disassembler: add support for the Homemade CPU to my existing [emulator-8-bit][1] application.

- [10-Oct-2025] The emulator and disassembler have been implemented, and some manual tests have been run. Still need to run more complete tests and create some automated test actions.

Initial circut:

- Program counter to access memory and load into `AC`
- LEDs to show value of `AC` and `PC`
- Clock circuit with debounced toggle switch

## History

| Date | Action |
| ---- | ------ |
| Sep-2025 | Document project design goals. Set up repo. |
| Oct-2025 | Initial CPU design: registers, addressing modes, opcode size and format, instruction set. |
|          | Create wiki. |
|          | Implement [emulator][1]: disassembler, emulator, automated tests. |
|          | Update design to version 2 to use 8-bit instructions instead of 16-bit. |
|          | - Updated [emulator][1] to support design version 2. |

[1]: https://github.com/Andy4495/emulator-8-bit
[2]: https://github.com/Andy4495/Homemade-CPU/wiki
[3]: https://github.com/Andy4495/Homemade-CPU/wiki/Blog
