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

### User Accessible

|      |                     |                                                                         |
| -----| ------------------- | ----------------------------------------------------------------------- |
| `AC` | Accumulator         |                                                                         |
| `FL` | Flags               | ZERO (`Z`), CARRY (`C`) , SIGN (`S`), OVERFLOW (`V`)                    |
| `SH` | Stack Pointer MSB   |                                                                         |
| `SL` | Stack Pointer LSB   |                                                                         |
| `MH` | Memory Pointer MSB  | MSB of memory location use by `LOAD` and `STOR` instructions            |
| `JH` | Jump Address MSB    | MSB of jump to address for the for `JUMP` and `JPxx` instructions       |
| `JL` | Jump Address LSB    | LSB of jump to address; only used by the RETU instruction               |
| `PC` | Program Counter MSB | This is a 16-bit value, indirectly accessible through jump instructions |

`SH` and `SL` are the MSB and LSB of the 16-bit `SP` register.

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

Register Indirect or Indexed addressing modes may be supported at a future time, either by the Assembler or the CPU directly.

## Basic Instructions

I chose 4-character fixed-length mnemonics to simplify the assembler implementation.

The Flags column indicates how flags are affected by the operation:

  `-`: Flag is not affected  
  `0`: Flag is cleared  
  `1`: Flag is set  
  `Z`, `C`, `S`, `V`: Flag value depends on result of operation  

| Mnemonic | Description                              | Mode          | `ZCSV` (Flags) | Notes |
| -------- | ---------------------------------------- | ------------- | -------------- | ----- |
| `LOAD`   | Immediate, memory, or register into `AC` | IMM, DIR, REG | `----`         |       |
| `STOR`   | `AC` into memory or register             | DIR, REG      | `----`         |       |
| `PUSH`   | Push register onto the stack             | REG, IMM      | `----`         |       |
| `POPP`   | Pop stack into register                  | REG           | `----`         |       |
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
| `BITC`   | Clear a bit in `AC`                      | BIT           | `----`         |       |
| `BITS`   | Set a bit in `AC`                        | BIT           | `----`         |       |
| `CLRV`   | Clear Overflow flag bit                  | BIT           | `---0`         |       |
| `CLRS`   | Clear Sign flag bit                      | BIT           | `--0-`         |       |
| `CLRC`   | Clear Carry flag bit                     | BIT           | `-0--`         |       |
| `CLRZ`   | Clear Zero flag bit                      | BIT           | `0---`         |       |
| `SETV`   | Set Overflow flag bit                    | BIT           | `---1`         |       |
| `SETS`   | Set Sign flag bit                        | BIT           | `--1-`         |       |
| `SETC`   | Set Carry flag bit                       | BIT           | `-1--`         |       |
| `SETZ`   | Set Zero flag bit                        | BIT           | `1---`         |       |
| `JPVC`   | Jump if Overflow flag is clear           | IMM           | `----`         |       |
| `JPSC`   | Jump if Sign flag is clear               | IMM           | `----`         |       |
| `JPCC`   | Jump if Carry flag is clear              | IMM           | `----`         |       |
| `JPZC`   | Jump if Zero flag is clear               | IMM           | `----`         |       |
| `JPVS`   | Jump if Overflow flag is set             | IMM           | `----`         |       |
| `JPSS`   | Jump if Sign flag is set                 | IMM           | `----`         |       |
| `JPCS`   | Jump if Carry flag is set                | IMM           | `----`         |       |
| `JPZS`   | Jump if Zero flag is set                 | IMM           | `----`         |       |
| `JUMP`   | Unconditional jump                       | IMM, REG      | `----`         | Register mode uses `JL` as a Return instruction. |
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
| `XNOR`   | Bitwise XNOR on `AC`                     | `Z000`         | `XORR` then `NOTT` |
| `LJMP`   | Long jump                                | ----           | Combines `LOAD JH,#MSB` and `JUMP #LSB` into an easier-to-read single instruction |
| `CALL`   | Call subroutine                          | ----           | Push `PC`, `FL`, `AC`, `MH`, `JH` to stack and jump to location |
| `RETU`   | Return from subroutine                   | ----           | Pop `PC`, `FL`, `AC`, `MH`, `JH` from stack and continue at PC  |
| `CAZS`   | Call subroutine if Zero flag set         | ----           |       |
| `CAZC`   | Call subroutine if Zero flag clear       | ----           |       |
| `CACS`   | Call subroutine if Carry flag set        | ----           |       |
| `CACC`   | Call subroutine if Carry flag clear      | ----           |       |
| `CASS`   | Call subroutine if Sign flag set         | ----           |       |
| `CASC`   | Call subroutine if Sign flag clear       | ----           |       |
| `CAVS`   | Call subroutine if Overflow  flag set    | ----           |       |
| `CAVC`   | Call subroutine if Overflow  flag clear  | ----           |       |

The `CALL` instruction needs to calculate the "future" PC value at the end of the call sequence (probably 16 bytes later than current PC), and then use the immediate mode of `PUSH` to store the return address to the stack.

The `RETU` instruction needs pop the return address to `JH` and `JL` and then use the register mode version of the `JUMP` command.

Index mode instructions may be added later. These aren't strictly necessary, but make using arrays a lot easier.

## Opcode Format

The CPU will use fixed-length 16-bit instructions. The MSB will be the opcode and the LSB the operand.

There are four groups of instructions:

- Data movement (ALU not required): `LOAD`, `STOR`, `PUSH`, `POPP`
- Data manipulation: `ADDD`, `SUBB`, `COMP`, `ANDD`, `ORRR`, `NOTT`, `XORR`, `NAND`, `NORR`, `SHxx`, `Rxxx`
- Bit operations: `BITS`, `BITC`, `SETx`, `CLRx`
- Program control: `JUMP`, `JPxx`, `HALT`, `NOOP`

### Instruction Group

Bits 7 and 6 of the opcode indicate the instruction group:

```text
00 - Data movement
01 - Data manipulation
10 - Bit operations
11 - Program control
```

#### Data Movement (`00 mmm rrr`)

##### `mmm`

Bits 5 to 3 (`mmm`) indicate the movement type:

```text
000 - LOAD Immediate
001 - LOAD Direct
010 - LOAD Register
011 - STOR Direct
100 - STOR Register
101 - PUSH Register
110 - PUSH Immediate
111 - POPP Register
```

#### `rrr`

Bits 2 to 0 (`rrr`) depend on the addressing mode.

- For Register addressing mode, the bits select the register (source for `LOAD` and `PUSH`, destination for `STOR` and `POPP`):

```text
000 - AC
001 - FL
010 - SH
011 - SL
100 - MH
101 - JH
110 - JL
```

- For other addressing modes, the bits are unused.

#### Data manipulation (`01 m iiii x`)

##### `m`

Bit 5 (`m`) indicates the addressing mode:

```text
0 - Immediate
1 - Indirect
```

##### `iiii`

Bits 4 to 1 (`iiii`) select the specific operation.

##### `x`

Bit 0 is unused.

#### Bit Operations (`10 r v bbb y`)

##### `r`

Bit 5 (`r`) selects the register that is being operated on:

```text
0 - AC
1 - FL
```

##### `v`

Bit 4 (`v`) selects the value (1 for set or 0 for clear) of the operation.

##### `bbb`

Bits 3 to 1 (`bbb`) select the bit position that is updated.

##### `y`

Bit 0 is unused.

#### Program Control (`11 u jjjj z`)

##### `u`

Bit 5 (`u`) selects whether the operation is unconditional or conditional.

##### `jjjj`

Bits 4 to 1 (`jjjj`) select the specific operation.

##### `z`

Bit 0 is unused.

## Opcode List

Note that the opcode for `HALT` was intentionally chosen to be `0xFF`.

Opcodes which are not listed are reserved for future use. Functionality of unlisted opcodes should not be assumed to work the same in future iterations of the CPU.

```text
Data Movement Instructions
                            Bit fields
Opcode  Mnemonic    Mode    00   mmm rrr
------  ---------   ----    ------------
00      LOAD #dd    IMM     00   000 000
08      LOAD (mm)   DIR     00   001 000
10      LOAD AC     REG     00   010 000
11      LOAD FL     REG     00   010 001
12      LOAD SH     REG     00   010 010
13      LOAD SL     REG     00   010 011
14      LOAD MH     REG     00   010 100
15      LOAD JH     REG     00   010 101
16      LOAD JL     REG     00   010 110
18      STOR (mm)   DIR     00   011 000
20      STOR AC     REG     00   100 000
21      STOR FL     REG     00   100 001
22      STOR SH     REG     00   100 010
23      STOR SL     REG     00   100 011
24      STOR MH     REG     00   100 100
25      STOR JH     REG     00   100 101
26      STOR JL     REG     00   100 110
28      PUSH AC     REG     00   101 000
29      PUSH FL     REG     00   101 001
2A      PUSH SH     REG     00   101 010
2B      PUSH SL     REG     00   101 011
2C      PUSH MH     REG     00   101 100
2D      PUSH ML     REG     00   101 101
2E      PUSH JL     REG     00   101 110
30      PUSH #dd    IMM     00   110 000
38      POPP AC     REG     00   111 000
39      POPP FL     REG     00   111 001
3A      POPP SH     REG     00   111 010
3B      POPP SL     REG     00   111 011
3C      POPP MH     REG     00   111 100
3D      POPP JH     REG     00   111 101
3E      POPP JL     REG     00   111 110

Data Manipulation Instructions
                            Bit fields
Opcode  Mnemonic    Mode    01  m iiii x
------  ---------   ----    ------------
40      COMP #dd    IMM     01  0 0000 0
42      SUBB #dd    IMM     01  0 0001 0
44      ADDD #dd    IMM     01  0 0010 0
50      ANDD #dd    IMM     01  0 1000 0
52      ORRR #dd    IMM     01  0 1001 0
54      NOTT #dd    IMM     01  0 1010 0
56      XORR #dd    IMM     01  0 1011 0
58      NAND #dd    IMM     01  0 1100 0
59      NORR #dd    IMM     01  0 1101 0
60      SHRL        IND     01  1 0000 0
62      SHLL        IND     01  1 0001 0
64      SHRA        IND     01  1 0010 0
68      ROTR        IND     01  1 0100 0
6A      RRTC        IND     01  1 0101 0
6C      ROTL        IND     01  1 0110 0
6E      RLTC        IND     01  1 0111 0

Bit Operation Instructions
                            Bit fields
Opcode  Mnemonic    Mode    10 r v bbb y
------  ---------   ----    ------------
80      BITC 0      BIT     10 0 0 000 0
82      BITC 1      BIT     10 0 0 001 0
84      BITC 2      BIT     10 0 0 010 0
86      BITC 3      BIT     10 0 0 011 0
88      BITC 4      BIT     10 0 0 100 0
8A      BITC 5      BIT     10 0 0 101 0
8C      BITC 6      BIT     10 0 0 110 0
8E      BITC 7      BIT     10 0 0 111 0
90      BITS 0      BIT     10 0 1 000 0
92      BITS 1      BIT     10 0 1 001 0
92      BITS 2      BIT     10 0 1 010 0
92      BITS 3      BIT     10 0 1 011 0
92      BITS 4      BIT     10 0 1 100 0
92      BITS 5      BIT     10 0 1 101 0
92      BITS 6      BIT     10 0 1 110 0
92      BITS 7      BIT     10 0 1 111 0
A0      CLRV        BIT     10 1 0 000 0
A2      CLRS        BIT     10 1 0 001 0
A4      CLRC        BIT     10 1 0 010 0
A6      CLRZ        BIT     10 1 0 011 0
B0      SETV        BIT     10 1 1 000 0
B2      SETS        BIT     10 1 1 001 0
B4      SETC        BIT     10 1 1 010 0
B6      SETZ        BIT     10 1 1 011 0

Program Control Instructions
                            Bit fields
Opcode  Mnemonic    Mode    11  u jjjj z
------  ---------   ----    ------------
C0      JPVC        IMM     11  0 0000 0
C2      JPSC        IMM     11  0 0001 0
C4      JPCC        IMM     11  0 0010 0
C6      JPZC        IMM     11  0 0011 0
D0      JPVS        IMM     11  0 1000 0
D2      JPSS        IMM     11  0 1001 0
D4      JPCS        IMM     11  0 1010 0
D6      JPZS        IMM     11  0 1011 0
E0      JUMP        IMM     11  1 0000 0
E6      JUMP        REG     11  1 0011 0  ; This only works with the `JL` register
FC      NOOP        N/A     11  1 1110 0
FF      HALT        N/A     11  1 1111 1
```

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
