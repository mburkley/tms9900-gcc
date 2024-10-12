/*
 * Copyright (c) 2004-2024 Mark Burkley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __CPU_H
#define __CPU_H

#include <iostream>
#include <cstdlib>

/*  Define addressing modes, optypes and opcodes.  These are used by the disassembler as well */

#define AMODE_NORMAL    0
#define AMODE_INDIR     1
#define AMODE_SYM       2
#define AMODE_INDIRINC  3

#define OPTYPE_IMMED    1
#define OPTYPE_SINGLE   2
#define OPTYPE_SHIFT    3
#define OPTYPE_JUMP     4
#define OPTYPE_DUAL1    5
#define OPTYPE_DUAL2    6

#define OP_LI   0x0200
#define OP_AI   0x0220
#define OP_ANDI 0x0240
#define OP_ORI  0x0260
#define OP_CI   0x0280
#define OP_STWP 0x02A0
#define OP_STST 0x02C0
#define OP_LWPI 0x02E0
#define OP_LIMI 0x0300

#define OP_RTWP 0x0380

#define OP_BLWP 0x0400
#define OP_B	0x0440
#define OP_X	0x0480
#define OP_CLR	0x04C0
#define OP_NEG	0x0500
#define OP_INV	0x0540
#define OP_INC	0x0580
#define OP_INCT 0x05C0
#define OP_DEC	0x0600
#define OP_DECT 0x0640
#define OP_BL	0x0680
#define OP_SWPB 0x06C0
#define OP_SETO 0x0700
#define OP_ABS	0x0740

#define OP_SRA  0x0800
#define OP_SRL  0x0900
#define OP_SLA  0x0A00
#define OP_SRC  0x0B00

#define OP_JMP  0x1000
#define OP_JLT  0x1100
#define OP_JLE  0x1200
#define OP_JEQ  0x1300
#define OP_JHE  0x1400
#define OP_JGT  0x1500
#define OP_JNE  0x1600
#define OP_JNC  0x1700
#define OP_JOC  0x1800
#define OP_JNO  0x1900
#define OP_JL   0x1A00
#define OP_JH   0x1B00
#define OP_JOP  0x1C00
#define OP_SBO  0x1D00
#define OP_SBZ  0x1E00
#define OP_TB   0x1F00

#define OP_COC  0x2000
#define OP_CZC  0x2400
#define OP_XOR  0x2800
#define OP_XOP  0x2C00
#define OP_LDCR 0x3000
#define OP_STCR 0x3400
#define OP_MPY  0x3800
#define OP_DIV  0x3C00

#define OP_SZC  0x4000
#define OP_SZCB 0x5000
#define OP_S    0x6000
#define OP_SB   0x7000
#define OP_C    0x8000
#define OP_CB   0x9000
#define OP_A    0xA000
#define OP_AB   0xB000
#define OP_MOV  0xC000
#define OP_MOVB 0xD000
#define OP_SOC  0xE000
#define OP_SOCB 0xF000

#define FLAG_LGT 0x8000
#define FLAG_AGT 0x4000
#define FLAG_EQ  0x2000
#define FLAG_C   0x1000
#define FLAG_OV  0x0800
#define FLAG_OP  0x0400
#define FLAG_XOP 0x0200
#define FLAG_MSK 0x000F

/*  Define a class for the CPU.  This is a standalone class with no dependencies.  It is 
 *  an abstract virtual class.  An instantiated class must derive from this and provide
 *  at a minimum methods for reading and writing memory.  There are optional void overridable
 *  methods for debug, disassembly, CRU and XOP. */

class TMS9900
{
public:
    // uint16_t read(uint16_t addr);
    void showStatus(void);
    void showStWord(void);
    uint16_t fetch (void);
    uint16_t decode (uint16_t data, uint16_t *type);
    void execute (uint16_t data);
    uint16_t getPC (void) { return _pc; }
    uint16_t getWP (void) { return _wp; }
    uint16_t getST (void) { return _st; }
    uint16_t getIntMask (void) { return _st & FLAG_MSK; }
    void interrupt (int level);
    void boot (void);
    void branch (uint16_t addr);
private:
    uint16_t _pc;
    uint16_t _wp;
    uint16_t _st;

    /*  Mandatory memory access overrides */
    virtual uint16_t _memReadW (uint16_t addr) = 0; // { return 0; }
    virtual uint8_t _memReadB (uint16_t addr) = 0; // { return 0; }
    virtual void _memWriteW (uint16_t addr, uint16_t data) = 0; // { }
    virtual void _memWriteB (uint16_t addr, uint8_t data) = 0; // { }

    /*  Optional debug */
    virtual void _debug (const char *s, ...) {}

    /*  Optional disassembly hooks */
    virtual uint16_t _unasmPreExec (uint16_t pc, uint16_t data, uint16_t type, uint16_t opcode) { return 0; }
    virtual void _unasmPostExec (const char *s, ...) {}
    virtual void _unasmEndLine (void) {}

    /*  Optional interrupt handlers */
    virtual int _interruptLevel (int mask) { return 0; }
    virtual void _halt (const char *s) { std::cerr << s; exit(1); }

    /*  Optional CRU handlers */
    virtual void _cruBitOutput (uint16_t base, uint16_t offset, uint8_t state) {}
    virtual void _cruMultiBitSet (uint16_t base, uint16_t data, int nBits) {}
    virtual uint16_t _cruMultiBitGet (uint16_t base, uint16_t offset) { return 0; }
    virtual uint8_t _cruBitGet (uint16_t base, int8_t bitOffset) { return 0; }

    /*  Optional XOP handlers */
    virtual void _xop (uint8_t vector, uint16_t data) {}

    /*  private methods implemented in cc */
    void _blwp (uint16_t addr);
    void _rtwp (void);
    void _jumpAnd (uint16_t setMask, uint16_t clrMask, uint16_t offset);
    void _jumpOr (uint16_t setMask, uint16_t clrMask, uint16_t offset);
    void _statusCarry (bool condition);
    void _statusOverflow (bool condition);
    void _statusEqual (bool condition);
    void _statusLogicalGreater (bool condition);
    void _statusArithmeticGreater (bool condition);
    void _statusParity (uint8_t value);
    char *_outputStatus (void);
    void _compareWord (uint16_t sData, uint16_t dData);
    void _compareByte (uint16_t sData, uint16_t dData);
    uint16_t _operandDecode (uint16_t mode, uint16_t reg, bool isByte);
    uint16_t _operandFetch (uint16_t mode, uint16_t reg, uint16_t addr, bool isByte, bool doFetch);
    void _executeImmediate (uint16_t opcode, uint16_t reg);
    void _executeSingle (uint16_t opcode, uint16_t mode, uint16_t reg);
    void _executeShift (uint16_t opcode, uint16_t reg, uint16_t count);
    void _executeJump (uint16_t opcode, int16_t offset);
    void _executeDual1 (uint16_t opcode, uint16_t dReg, uint16_t sMode, uint16_t sReg);
    void _executeDual2 (uint16_t opcode, uint16_t dMode, uint16_t dReg,
                                 uint16_t sMode, uint16_t sReg, bool isByte);
};

#endif

