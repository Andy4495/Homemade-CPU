# Homemade CPU

Designing my own CPU.

This is going to be way better than the mess of wires I created for my microprocessor design class many years ago.

See the [Wiki](https://github.com/Andy4495/Homemade-CPU/wiki) for design details and documentation.

## Initial Design Goals

- 8-bit data, 16-bit address bus
- Implement with readily-available, off-the-shelf logic chips
  - 74HCxx-series
- Based on RISC principles
  - Each instruction should execute in one clock cycle (not including fetch)
  - Fixed-length instructions with a simple encoding
    - This probably means 16-bit instructions (1 byte opcode, 1 byte operand)
    - Accessing memory requires a separate load into a high address register
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
- Track development through a blog
  - Document each step in the process: why a design decision was made, why something was later changed
  - Use a tracker table to show progress of my various design goals (implemnted, not implemted yet, will not implement)

## Next Steps

Initial circut:

- Program counter to access memory and load into `AC`
- LEDs to show value of `AC` and `PC`
- Clock circuit with debounced toggle switch

## References

### Books

| Author | Title | Publisher | Edition | Year | ISBN |
| ------ | ----- | --------- | ------- | ---- | ---- |
| Hamacher, V. Carl; Zvonko, G Vranesic; Zaky, Safwat G | *Computer Organization*.         | McGraw-Hill     | 3rd | 1990 | 0-07-100742-3 |
| Dewar, Robert B. K.; Smosna, Matthew | *Microprocessors: A Programmer's View*            | McGraw-Hill     | 1st | 1990 | 0-07-016639-0 |
| Hayes, John P. | *Computer Architecture and Organization*                                | McGraw-Hill     | 2nd | 1988 | 0-07-027366-9 |
| Hennessy, John L.; Patterson, David A. | *Computer Architecture: A Quatitative Approach* | Morgan Kaufmann | 1st | 1990 | 1-55860-069-8 |
| Rafiquzzaman, Mohamed | *Microprocessors and Microcomputer-Based System Design*          | CRC Press       | 1st | 1990 | 0-8493-4275-9 |

### Online

- [Gigatron][1] TTL Microcomputer
- Ben Eater's "Building an 8-bit Breadboard Computer" [playlist][2]

[1]: https://gigatron.io
[2]: https://www.youtube.com/watch?v=HyznrdDSSGM&list=PLowKtXNTBypGqImE405J2565dvjafglHU
