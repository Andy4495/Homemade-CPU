# Homemade CPU

Designing my own CPU.

This is going to be way better than the mess of wires I created for my microprocessor design class many years ago.

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

## Registers

### User Accessible Directly

|      |                     |                                                               |
| -----| ------------------- | ------------------------------------------------------------- |
| `AC` | Accumulator         |                                                               |
| `FL` | Flags               | ZERO (`Z`), CARRY (`C`) , SIGN (`S`), OVERFLOW (`V`)          |
| `SH` | Stack Pointer MSB   |                                                               |
| `SL` | Stack Pointer LSB   |                                                               |
| `MH` | Memory Pointer MSB  | MSB of memory location use by LOAD and STOR instructions      |
| `JH` | Jump Address MSB    | MSB of jump to address for the for JUMP and JPxx instructions |

### User Accessible Indirectly

|      |                 |                                                                  |
| ---- | --------------- | ---------------------------------------------------------------- |
| `PC` | Program Counter | Implemented with a built-in counter chip like 74HC161 or 74HC163 |

There are no opcodes that take `PC` as an operand, but the `JUMP` and `JPxx` opcodes update the value of `PC`.

### Internal, Non-User-Accessible

|      |                 |                                 |
| ---- | --------------- | ------------------------------- |
| `IR` | Instruction     | Last opcode loaded from memory  |
| `OR` | Operand         | Last operand loaded from memory |

The `OR` (Operand Register) can be thought of as the second accumulator input to the ALU.

## Addressing Modes

| Mode      | Abbreviation | Description                                                           |
| --------- | ------------ | --------------------------------------------------------------------- |
| Register  | REG          | Operand is a CPU register selected by the instruction                 |
| Immediate | IMM          | Operand is included in instruction itself; denoted with `#`           |
| Direct    | DIR          | Instruction contains the address of operand; denoted with `(` and `)` |
| Indirect  | IND          | Operand is implicit in the instruction                                |
| Bit       | BIT          | Manipulate a specific bit within a register                           |

There are  Register Indirect or Indexed addressing modes are not listed above because there are currently no plans to support thees modes in the CPU or assembler.

## Basic Instructions

I chose 4-character mnemonics to simplify the assembler implementation.

The Flags column indicates how flags are affected by the operation:

  `-`: Flag is not affected
  `0`: Flag is cleared
  `1`: Flag is set
  `Z`, `C`, `S`, `V`: Flag value depends on result of operation

| Mnemonic | Description                              | Mode          | `ZCSV` (Flags) | Notes |
| -------- | ---------------------------------------- | ------------- | -------------- | ----- |
| `LOAD`   | Immediate, memory, or register into `AC` | IMM, DIR, REG | `----`         |       |
| `STOR`   | `AC` into memory or register             | DIR, REG      | `----`         |       |
| `PUSH`   | Push `AC` onto the stack                 | IND           | `----`         |       |
| `POPS`   | Pop stack into `AC`                      | IND           | `----`         |       |
| `COMP`   | Subtract from `AC`, result discarded     | IMM           | `ZCSV`         |       |
| `SUBB`   | Subtract from `AC`                       | IMM           | `ZCSV`         |       |
| `ADDD`   | Add to `AC`                              | IMM           | `ZCSV`         |       |
| `ANDD`   | Bitwise AND on `AC`                      | IMM           | `Z000`         |       |
| `ORRR`   | Bitwise OR on `AC`                       | IMM           | `Z000`         |       |
| `NOTT`   | Bitwise NOT on `AC`                      | IMM           | `Z000`         | 1's complement     |
| `XORR`   | Bitwise XOR on `AC`                      | IMM           | `Z000`         |       |
| `NAND`   | Bitwise NAND on `AC`                     | IMM           | `Z000`         |       |
| `NORR`   | Bitwise NOR on `AC`                      | IMM           | `Z000`         |       |
| `SHRL`   | Shift right logical on `AC`              | IND           | `ZC00`         | Bit 0 is shifted into Carry flag; bit 7 is set to 0 |
| `SHLL`   | Shift left logical on `AC`               | IND           | `ZC00`         | Bit 7 is shifted into Carry flag; bit 0 is set to 0 |
| `SHRA`   | Shift right arithmetic on `AC`           | IND           | `ZCS0`         | Bit 7 is shifted into itself and bit 6; bit 0 is shifted into Carry flag |
| `ROTR`   | Rotate right  on `AC`                    | IND           | `ZCS0`         | Bit 0 is shifed into Carry flag |
| `RRTC`   | Rotate right through carry  on `AC`      | IND           | `ZCS0`         | Carry flag shifted into bit 7; bit 0 shifted into Carry flag |
| `ROTL`   | Rotate left  on `AC`                     | IND           | `ZCS0`         | Bit 7 is shifted into Carry flag |
| `RLTC`   | Rotate left through carry  on `AC`       | IND           | `ZCS0`         | Carry flag shifted into bit 0; but 7 shifted into Carry flag |
| `BITS`   | Set a bit in `AC`                        | BIT           | `----`         |       |
| `BITC`   | Clear a bit in `AC`                      | BIT           | `----`         |       |
| `SETZ`   | Set Zero flag bit                        | BIT           | `----`         |       |
| `CLRZ`   | Clear Zero flag bit                      | BIT           | `----`         |       |
| `SETC`   | Set Carry flag bit                       | BIT           | `----`         |       |
| `CLRC`   | Clear Carry flag bit                     | BIT           | `----`         |       |
| `SETS`   | Set Sign flag bit                        | BIT           | `----`         |       |
| `CLRS`   | Clear Sign flag bit                      | BIT           | `----`         |       |
| `SETV`   | Set Overflow flag bit                    | BIT           | `----`         |       |
| `CLRV`   | Clear Overflow flag bit                  | BIT           | `----`         |       |
| `JPZS`   | Jump if Zero flag is set                 | IMM           | `----`         |       |
| `JPZC`   | Jump if Zero flag is clear               | IMM           | `----`         |       |
| `JPCS`   | Jump if Carry flag is set                | IMM           | `----`         |       |
| `JPCC`   | Jump if Carry flag is clear              | IMM           | `----`         |       |
| `JPSS`   | Jump if Sign flag is set                 | IMM           | `----`         |       |
| `JPSC`   | Jump if Sign flag is clear               | IMM           | `----`         |       |
| `JPVS`   | Jump if Overflow flag is set             | IMM           | `----`         |       |
| `JPVC`   | Jump if Overflow flag is clear           | IMM           | `----`         |       |
| `JUMP`   | Unconditional jump                       | IMM           | `----`         |       |
| `NOOP`   | No operation                             | N/A           | `----`         |       |
| `HALT`   | Stop fetching new instructions           | N/A           | `----`         |       |

The "rotate through carry" instructions are included in order to simplify multi-byte shift operations. For example, if you wanted to divide a signed 16-bit value by 2, you could use the following:

```text
    LOAD A,(MSB)
    SHRA A      ; Previous bit 0 of MSB is now in Carry flag
    STOR A,(MSB)
    LOAD A,(LSB)
    RRTC A      ; Bit 0 from MSB is shifted into bit 7 of LSB through Carry flag
    STOR A,(LSB)
```

The bit operations (`BITx`, `SETx`, `CLRx`) are included at this level because they do not impact the flags. Simulating these operations with AND or OR would have the side effect of changing the flag bits.

The `HALT` instruction may also signal an externally accessible hardware line that the CPU is halted.

## Instructions Supported by Assembler

In addition to the opcodes listed above that are directly supported by the CPU, the assembler will also support the following instructions. These can easily be implemented by stringing together several basic CPU opcodes.

| Mnemonic | Description                              | Flags (ZCSV)   | Notes |
| -------- | ---------------------------------------- | -------------- | ----- |
| `AD16`   | Add two 16 bit values                    | `ZCSV`         |       |
| `SU16`   | Subtract two 16 bit values               | `ZCSV`         |       |
| `CM08`   | 2's complement 8 bit value               | `Z0S0`         |       |
| `CM16`   | 2's complement 16 bit value              | `Z0S0`         |       |
| `XNOR`   | Bitwise XNOR on `AC`                     | `Z000`         | `XORR` then `NOTT`     |
| `LJMP`   | Long jump                                | ----           | Combines `LOAD JH,#MSB` and `JUMP #LSB` into an easier-to-read single instruction |
| `CALL`   | Call subroutine                          | ----           | Push PC, F, A, MH, JH to stack and jump to location |
| `RETN`   | Return from subroutine                   | ----           | Pop PC, F, A, MH, JH from stack and continue at PC |
| `CAZS`   | Call subroutine if Zero flag set         | ----           |       |
| `CAZC`   | Call subroutine if Zero flag clear       | ----           |       |
| `CACS`   | Call subroutine if Carry flag set        | ----           |       |
| `CACC`   | Call subroutine if Carry flag clear      | ----           |       |
| `CASS`   | Call subroutine if Sign flag set         | ----           |       |
| `CASC`   | Call subroutine if Sign flag clear       | ----           |       |
| `CAVS`   | Call subroutine if Overflow  flag set    | ----           |       |
| `CAVC`   | Call subroutine if Overflow  flag clear  | ----           |       |

Index mode instructions may be added later. These aren't strictly necessary, but make using arrays a lot easier.

## Opcode Format

The CPU will used fixed-length 16-bit instructions. The MSB will be the opcode and the LSB the operand.

There are three general types of instructions (2 bits):

- Data movement (ALU not required): `LOAD`, `STOR`, `PUSH`, `POPS`
- Data manipulation: `ADDD`, `SUBB`, `COMP`, `ANDD`, `ORRR`, `NOTT`, `XORR`, `NAND`, `NORR`, shift, rotate
- Program control: `JUMP`, `JPxx`, `HALT`, `NOP`
- Bit operations: `BITS`, `BITC`, `SETx`, `CLRx`

### Opcode Bit-Field Breakdown

#### Data movement (0b00xxxxxx)

`LOAD`: 3 modes
`STOR`: 2 modes
`PUSH`: 1 mode
`POPS`: 1 mode

Seven operations: 3 bits

Source register for `LOAD` register mode: 3 bits

#### Data manipulation (0b01xxxxxx)

Mode (immediate or indirect): 1 bit
Instruction: 4 bits (either one of 7 IND or one of 9 IMM)

#### Bit Operations (0b10xxxxxx)

These instructions act on either `AC` or `AF`: 1 bit to select register

Bit position: 3 bits

Bit value: 1 bit

#### Program Control (0b11xxxxxx)

Conditional/Unconditional: 1 bit

Unconditional: 3 bits to select one of 8 possible instructions

Unconditional: 2 bits to select `JUMP`, `NOOP`, `HALT`

### Opcode Definitions

In progress...

## Next Steps

Opcode table

Initial circut:

- Program counter to access memory and load into `AC`
- LEDs to show value of `AC` and `PC`
- Clock circuit with debounced toggle switch

## References

### Books

| Author | Title | Publisher | Edition | Year | ISBN | PDF | Notes |
| ------ | ----- | --------- | ------- | ---- | ---- | --- | ----- |
| Hamacher, V. Carl; Zvonko, G Vranesic; Zaky, Safwat G | *Computer Organization* | McGraw-Hill | 3rd | 1990 | 0-07-100742-3 | N/A | |

### Online

- [Gigatron][1] TTL Microcomputer
- Ben Eater's "Building an 8-bit Breadboard Computer" [playlist][2]

[1]: https://gigatron.io
[2]: https://www.youtube.com/watch?v=HyznrdDSSGM&list=PLowKtXNTBypGqImE405J2565dvjafglHU
