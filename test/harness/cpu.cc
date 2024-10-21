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

/*
 *  Implements TMS9900 CPU
 */

#include "types.h"

#include "cpu.h"

#if 0
#include "types.h"
#include "cpu.h"
#include "interrupt.h"
#include "cru.h"
#include "unasm.h"
#include "mem.h"
#include "trace.h"
#endif

typedef struct
{
    uint16_t index;
    uint16_t type;
    uint16_t opMask;
}
OpGroup;

static OpGroup opGroup[64] =
{
    { 0x0000, OPTYPE_IMMED,   0xFFE0 }, // LI, AI, ANDI, ORI, CI, STWP,
                                        // STST, LWPI, LIMI, RTWP
    { 0x0400, OPTYPE_SINGLE,  0xFFC0 }, // BLWP, B, X, CLR, NEG, INV,
                                        // INC(T), DEC(T), BL, SWPB, SETO, ABS
    { 0x0800, OPTYPE_SHIFT,   0xFF00 }, // SRA, SRL, SLA, SRC
    { 0x0C00, 0,              0xFFFF },
    { 0x1000, OPTYPE_JUMP,    0xFF00 }, // JMP, JLT, JLE, JEQ
    { 0x1400, OPTYPE_JUMP,    0xFF00 }, // JHE, JGT, JNE, JNC
    { 0x1800, OPTYPE_JUMP,    0xFF00 }, // JOC, JNO, JL, JH
    { 0x1C00, OPTYPE_JUMP,    0xFF00 }, // JOP, SBO, SBZ, TZ
    { 0x2000, OPTYPE_DUAL1,   0xFC00 }, // COC
    { 0x2400, OPTYPE_DUAL1,   0xFC00 }, // CZC
    { 0x2800, OPTYPE_DUAL1,   0xFC00 }, // XOR
    { 0x2C00, OPTYPE_DUAL1,   0xFC00 }, // XOP
    { 0x3000, OPTYPE_DUAL1,   0xFC00 }, // LDCR
    { 0x3400, OPTYPE_DUAL1,   0xFC00 }, // STCR
    { 0x3800, OPTYPE_DUAL1,   0xFC00 }, // MPY
    { 0x3C00, OPTYPE_DUAL1,   0xFC00 }, // DIV
    { 0x4000, OPTYPE_DUAL2,   0xF000 }, // SZC
    { 0x4400, OPTYPE_DUAL2,   0xF000 },
    { 0x4800, OPTYPE_DUAL2,   0xF000 },
    { 0x4C00, OPTYPE_DUAL2,   0xF000 },
    { 0x5000, OPTYPE_DUAL2,   0xF000 }, // SZCB
    { 0x5400, OPTYPE_DUAL2,   0xF000 },
    { 0x5800, OPTYPE_DUAL2,   0xF000 },
    { 0x5C00, OPTYPE_DUAL2,   0xF000 },
    { 0x6000, OPTYPE_DUAL2,   0xF000 }, // S
    { 0x6400, OPTYPE_DUAL2,   0xF000 },
    { 0x6800, OPTYPE_DUAL2,   0xF000 },
    { 0x6C00, OPTYPE_DUAL2,   0xF000 },
    { 0x7000, OPTYPE_DUAL2,   0xF000 }, // SB
    { 0x7400, OPTYPE_DUAL2,   0xF000 },
    { 0x7800, OPTYPE_DUAL2,   0xF000 },
    { 0x7C00, OPTYPE_DUAL2,   0xF000 },
    { 0x8000, OPTYPE_DUAL2,   0xF000 }, // C
    { 0x8400, OPTYPE_DUAL2,   0xF000 },
    { 0x8800, OPTYPE_DUAL2,   0xF000 },
    { 0x8C00, OPTYPE_DUAL2,   0xF000 },
    { 0x9000, OPTYPE_DUAL2,   0xF000 }, // CB
    { 0x9400, OPTYPE_DUAL2,   0xF000 },
    { 0x9800, OPTYPE_DUAL2,   0xF000 },
    { 0x9C00, OPTYPE_DUAL2,   0xF000 },
    { 0xA000, OPTYPE_DUAL2,   0xF000 }, // A
    { 0xA400, OPTYPE_DUAL2,   0xF000 },
    { 0xA800, OPTYPE_DUAL2,   0xF000 },
    { 0xAC00, OPTYPE_DUAL2,   0xF000 },
    { 0xB000, OPTYPE_DUAL2,   0xF000 }, // AB
    { 0xB400, OPTYPE_DUAL2,   0xF000 },
    { 0xB800, OPTYPE_DUAL2,   0xF000 },
    { 0xBC00, OPTYPE_DUAL2,   0xF000 },
    { 0xC000, OPTYPE_DUAL2,   0xF000 }, // MOV
    { 0xC400, OPTYPE_DUAL2,   0xF000 },
    { 0xC800, OPTYPE_DUAL2,   0xF000 },
    { 0xCC00, OPTYPE_DUAL2,   0xF000 },
    { 0xD000, OPTYPE_DUAL2,   0xF000 }, // MOVB
    { 0xD400, OPTYPE_DUAL2,   0xF000 },
    { 0xD800, OPTYPE_DUAL2,   0xF000 },
    { 0xDC00, OPTYPE_DUAL2,   0xF000 },
    { 0xE000, OPTYPE_DUAL2,   0xF000 }, // SOCB
    { 0xE400, OPTYPE_DUAL2,   0xF000 },
    { 0xE800, OPTYPE_DUAL2,   0xF000 },
    { 0xEC00, OPTYPE_DUAL2,   0xF000 },
    { 0xF000, OPTYPE_DUAL2,   0xF000 }, // SOCB
    { 0xF400, OPTYPE_DUAL2,   0xF000 },
    { 0xF800, OPTYPE_DUAL2,   0xF000 },
    { 0xFC00, OPTYPE_DUAL2,   0xF000 }
};

//  Declare one global instance of the CPU
// TMS9900 cpu;

#define REGR(r) _memReadW(_wp+((r)<<1))
#define REGW(r,d) _memWriteW(_wp+((r)<<1),d)

uint16_t TMS9900::fetch (void)
{
    uint16_t ret;

    ret = _memReadW(_pc);
    _pc += 2;
    return ret;
}

void TMS9900::_blwp (uint16_t addr)
{
    uint16_t owp = _wp;
    uint16_t opc = _pc;

    _wp = _memReadW (addr);
    _pc = _memReadW (addr+2);

    _debug ("blwp @%x, wp=%x, pc=%x\n", addr, _wp, _pc);

    REGW (13, owp);
    REGW (14, opc);
    REGW (15, _st);
}

void TMS9900::_rtwp (void)
{
    _pc = REGR (14);
    _st = REGR (15);
    _wp = REGR (13);
}

/*  Jump if all bits in the set mask are set and all bits in the clear mask are
 *  clear */
void TMS9900::_jumpAnd (uint16_t setMask, uint16_t clrMask, uint16_t offset)
{
    if ((_st & setMask) == setMask &&
        (~_st & clrMask) == clrMask)
    {
        _unasmPostExec("st=%04X[s=%04X&&c=%04X], jump", _st, setMask,
        clrMask);
        _pc += offset << 1;
    }
}

/*  Jump if any bits in the set mask are set or any bits in the clear mask are
 *  clear */
void TMS9900::_jumpOr (uint16_t setMask, uint16_t clrMask, uint16_t offset)
{
    if ((_st & setMask) != 0 ||
        (~_st & clrMask) != 0)
    {
        _unasmPostExec("st=%04X[s=%04X||c=%04X], jump", _st, setMask,
        clrMask);
        _pc += offset << 1;
    }
}

void TMS9900::_statusCarry (bool condition)
{
    if (condition)
        _st |= FLAG_C;
    else
        _st &= ~FLAG_C;
}

void TMS9900::_statusOverflow (bool condition)
{
    if (condition)
        _st |= FLAG_OV;
    else
        _st &= ~FLAG_OV;
}

void TMS9900::_statusEqual (bool condition)
{
    if (condition)
        _st |= FLAG_EQ;
    else
        _st &= ~FLAG_EQ;
}

void TMS9900::_statusLogicalGreater (bool condition)
{
    if (condition)
        _st |= FLAG_LGT;
    else
        _st &= ~FLAG_LGT;
}

void TMS9900::_statusArithmeticGreater (bool condition)
{
    if (condition)
        _st |= FLAG_AGT;
    else
        _st &= ~FLAG_AGT;
}

void TMS9900::_statusParity (uint8_t value)
{
    bool oddParity = false;

    for (int i = 0; i < 8; i++)
        if (value & (1<<i))
            oddParity = !oddParity;

    // printf("OP %02x = %s\n", value, oddParity ? "ODD" : "EVEN");

    if (oddParity)
        _st |= FLAG_OP;
    else
        _st &= ~FLAG_OP;
}

char *TMS9900::_outputStatus (void)
{
    static char text[10];
    char *tp = text;
    int st = _st;

    *tp++ = '[';
    if (st & 0x8000) *tp++ = 'G';
    if (st & 0x4000) *tp++ = 'A';
    if (st & 0x2000) *tp++ = '=';
    if (st & 0x1000) *tp++ = 'C';
    if (st & 0x0800) *tp++ = 'O';
    if (st & 0x0400) *tp++ = 'P';
    if (st & 0x0200) *tp++ = 'X';
    *tp++=']';
    *tp++ = 0;

    return text;
}

void TMS9900::_compareWord (uint16_t sData, uint16_t dData)
{
    _statusEqual (sData == dData);
    _statusLogicalGreater (sData > dData);
    // _unasmPostExec("[AGT:%x>%x]", (int16_t)sData, (int16_t)dData);
    _statusArithmeticGreater ((int16_t) sData > (int16_t) dData);
    _unasmPostExec (_outputStatus());
}

void TMS9900::_compareByte (uint16_t sData, uint16_t dData)
{
    _statusEqual (sData == dData);
    _statusLogicalGreater (sData > dData);
    _statusArithmeticGreater ((int8_t) sData > (int8_t) dData);
    _unasmPostExec (_outputStatus());
}

uint16_t TMS9900::_operandDecode (uint16_t mode, uint16_t reg, bool isByte)
{
    uint16_t addr;

    switch (mode)
    {
    case AMODE_NORMAL:
        addr = _wp+(reg<<1);
        break;

    case AMODE_INDIR:
        addr = REGR(reg);
        break;

    case AMODE_SYM:
        addr = (uint16_t) (fetch() + (reg == 0 ? 0 : REGR(reg)));
        break;

    case AMODE_INDIRINC:
        addr = REGR(reg);
        REGW (reg, REGR(reg) + (isByte ? 1 : 2));
        break;

    default:
        _halt ("Bad operand mode");
    }

    return addr;
}

uint16_t TMS9900::_operandFetch (uint16_t mode, uint16_t reg, uint16_t addr, bool isByte, bool doFetch)
{
    uint16_t data = 0;

    if (isByte)
    {
        if (mode)
            _unasmPostExec("B:[%04X]", addr);
        else
            _unasmPostExec("R%d", reg);

        if (doFetch)
        {
            data = _memReadB (addr);
            _unasmPostExec("=%02X", data);
        }
    }
    else
    {
        if (mode)
            _unasmPostExec("W:[%04X]", addr);
        else
            _unasmPostExec("R%d", reg);

        if (doFetch)
        {
            data = _memReadW (addr);
            _unasmPostExec("=%04X", data);
        }
    }

    return data;
}

/*
 *  I M M E D I A T E S
 */

void TMS9900::_executeImmediate (uint16_t opcode, uint16_t reg)
{
    uint16_t immed;
    uint32_t data;

    switch (opcode)
    {
    case OP_LI:
        immed = fetch();
        REGW(reg,immed);
        break;

    case OP_AI:
        data = REGR(reg);
        immed = fetch();
        /*  Overflow if MSB(data)=MSB(Imm) && MSB(result) != MSB (data) */
        _statusOverflow ((data & 0x8000) == (immed & 0x8000) &&
                        ((data+immed) & 0x8000) != (data & 0x8000));
        _unasmPostExec ("R%d=%04X+%04X=%04X", reg, data, immed, data+immed);
        data += immed;
        _statusCarry (data >= 0x10000);
        data &= 0xffff;
        REGW(reg,data);
        _compareWord (data, 0);
        break;

    case OP_ANDI:
        data = REGR(reg);
        immed = fetch();
        _unasmPostExec ("R%d=%04X&%04X=%04X", reg, data, immed, data&immed);
        data &= immed;
        REGW(reg,data);
        _compareWord (data, 0);
        break;

    case OP_ORI:
        data = REGR(reg);
        immed = fetch();
        _unasmPostExec ("R%d=%04X|%04X=%04X", reg, data, immed, data|immed);
        data |= immed;
        REGW(reg,data);
        _compareWord (data, 0);
        break;

    case OP_CI:
        data = REGR(reg);
        _unasmPostExec ("R%d=%04X", reg, data);
        _compareWord (data, fetch());
        break;

    case OP_STST:
        immed = _st;
        _unasmPostExec ("R%d=%04X", reg, immed);
        REGW(reg,immed);
        break;

    case OP_STWP:
        immed = _wp;
        _unasmPostExec ("R%d=%04X", reg, immed);
        REGW(reg,immed);
        break;

    case OP_LWPI:
        immed = fetch();
        _wp = immed;
        break;

    case OP_LIMI:
        _st = (_st & ~FLAG_MSK) | fetch();
        break;

    case OP_RTWP:
        _rtwp ();
        _unasmPostExec ("pc=%04X", _pc);
        break;

    default:
        _halt ("Bad immediate opcode");
    }
}

void TMS9900::_executeSingle (uint16_t opcode, uint16_t mode, uint16_t reg)
{
    uint16_t addr;
    uint16_t param;

    addr = _operandDecode (mode, reg, false);

    if (mode)
        _unasmPostExec("W:[%04X]", addr);
    else
        _unasmPostExec("R%d", reg);

    switch (opcode)
    {
    case OP_BLWP:
        _unasmPostExec ("=%04X", addr);
        _blwp (addr);
        break;

    case OP_B:
        _unasmPostExec ("=%04X", addr);
        _pc = addr;
        break;

    case OP_X:
        param = _memReadW (addr);
        _debug ("X : recurse\n");
        execute (param);
        break;

    case OP_CLR:
        _memWriteW (addr, 0);
        break;

    case OP_NEG:
        param = _memReadW (addr);
        _statusCarry (param == 0x8000);
        _statusOverflow (param & 0x8000);
        param = -param;
        _unasmPostExec ("=%04X", param);
        _memWriteW (addr, param);
        _compareWord (param, 0);
        break;

    case OP_INV:
        param = ~_memReadW (addr);
        _unasmPostExec ("=%04X", param);
        _memWriteW (addr, param);
        _compareWord (param, 0);
        break;

    case OP_INC:
        param = _memReadW (addr);
        _statusOverflow ((param & 0x8000) == 0 &&
                        ((param + 1) & 0x8000) == 0x8000);
        _statusCarry (param == 0xFFFF);
        param += 1;
        _unasmPostExec ("=%04X", param);
        _memWriteW (addr, param);
        _compareWord (param, 0);
        break;

    case OP_INCT:
        param = _memReadW (addr);
        _statusOverflow ((param & 0x8000) == 0 &&
                        ((param + 2) & 0x8000) == 0x8000);
        _statusCarry ((param & 0xFFFE) == 0xFFFE);
        param += 2;
        if(addr&1)
        {
            param&=0xFF;
            _unasmPostExec ("=%02X", param);
            _memWriteB(addr,param);
            _compareByte (param, 0);
        }
        else
        {
            _unasmPostExec ("=%04X", param);
            _memWriteW (addr, param);
            _compareWord (param, 0);
        }
        break;

    case OP_DEC:
        param = _memReadW (addr);
        _statusCarry (param != 0);
        _statusOverflow ((param & 0x8000) == 0x8000 &&
                        ((param - 1) & 0x8000) == 0);
        param -= 1;
        _unasmPostExec ("=%04X", param);
        _memWriteW (addr, param);
        _compareWord (param, 0);
        break;

    case OP_DECT:
        param = _memReadW (addr);
        _statusCarry (param != 0 && param != 1);
        _statusOverflow ((param & 0x8000) == 0x8000 &&
                        ((param - 2) & 0x8000) == 0);
        param -= 2;
        /*  Not sure if this is strictly necessary, but in the ROM code there
         *  are several places where DECT is called on an odd address.  Does
         *  this mean only the low byte should be decremented?  Assume so for
         *  now.
         */
        if(addr&1)
        {
            param&=0xFF;
            _unasmPostExec ("=%02X", param);
            _memWriteB(addr,param);
            _compareByte (param, 0);
        }
        else
        {
            _unasmPostExec ("=%04X", param);
            _memWriteW (addr, param);
            _compareWord (param, 0);
        }
        break;

    case OP_BL:
        REGW(11, _pc);
        _pc = addr;
        break;

    case OP_SWPB:
        param = _memReadW (addr);
        param = SWAP(param);
        _unasmPostExec ("=%04X", param);
        _memWriteW (addr, param);
        _compareWord (param, 0);
        break;

    case OP_SETO:
        _memWriteW (addr, 0xFFFF);
        break;

    case OP_ABS:
        param = _memReadW (addr);
        _statusCarry (param == 0x8000);
        _statusOverflow (param & 0x8000);
        /*  AGT for ABS is unusual in that it takes the sign of the source into
         *  account and doesn't just do a comparison of the result to zero */
        _statusArithmeticGreater ((int8_t) param > 0);
        param = ((int16_t) param < 0) ? -param : param;
        _unasmPostExec ("=%04X", param);
        _memWriteW (addr, param);
        _statusEqual (param == 0);
        _statusLogicalGreater (param != 0);
        _unasmPostExec (_outputStatus());
        break;

    default:
        _halt ("Bad single opcode");
    }
}

void TMS9900::_executeShift (uint16_t opcode, uint16_t reg, uint16_t count)
{
    uint32_t u32;
    int32_t i32;

    if (count == 0)
        count = REGR(0) & 0x000F;

    if (count == 0)
        count = 16;

    switch (opcode)
    {
    case OP_SRA:
        i32 = REGR (reg) << 16;
        _unasmPostExec ("%04X=>", i32>>16);
        i32 >>= count;

        /* Set carry flag if last bit shifted is set */
        _statusCarry ((i32 & 0x8000) != 0);

        u32 = (i32 >> 16) & 0xffff;
        REGW (reg, u32);
        _compareWord (u32, 0);
        break;

    case OP_SRC:
        u32 = REGR (reg);
        _unasmPostExec ("%04X=>", u32);
        u32 |= (u32 << 16);
        _debug ("u32=%x\n", u32);
        u32 >>= count;
        _debug ("u32=%x\n", u32);

        /* Set carry flag if last bit shifted is set */
        _statusCarry ((u32 & 0x8000) != 0);

        u32 &= 0xffff;
        REGW (reg, u32);
        _compareWord (u32, 0);
        break;

    case OP_SRL:
        u32 = REGR (reg) << 16;
        _unasmPostExec ("%04X=>", u32>>16);
        u32 >>= count;

        /* Set carry flag if last bit shifted is set */
        _statusCarry ((u32 & 0x8000) != 0);

        u32 >>= 16;
        REGW (reg, u32);
        _compareWord (u32, 0);
        break;

    case OP_SLA:
        i32 = REGR (reg);
        _unasmPostExec ("%04X=>", i32);
        u32 = i32 << count;

        /* Set carry flag if last bit shifted is set */
        _statusCarry ((u32 & 0x10000) != 0);

        /* Set if MSB changes */
        _statusOverflow ((u32 & 0x8000) != (i32 & 0x8000));

        u32 &= 0xFFFF;
        REGW (reg, u32);
        _compareWord (u32, 0);
        break;

    default:
        _halt ("Bad shift opcode");
    }

    _unasmPostExec ("%04X", u32);
}

/*
 *  J U M P
 */
void TMS9900::_executeJump (uint16_t opcode, int16_t offset)
{
    switch (opcode)
    {
    case OP_JMP: _jumpAnd (0,        0,                  offset);    break;
    case OP_JLT: _jumpAnd (0,        FLAG_AGT | FLAG_EQ, offset);    break;
    case OP_JGT: _jumpAnd (FLAG_AGT, FLAG_EQ,                  offset);    break;
    case OP_JL:  _jumpAnd (0,        FLAG_LGT | FLAG_EQ, offset);    break;
    case OP_JLE: _jumpOr  (FLAG_EQ,  FLAG_LGT,           offset);    break;
    case OP_JH:  _jumpAnd (FLAG_LGT, FLAG_EQ,                  offset);    break;
    case OP_JHE: _jumpOr  (FLAG_LGT | FLAG_EQ, 0,            offset);    break;
    case OP_JNC: _jumpAnd (0,        FLAG_C,             offset);    break;
    case OP_JOC: _jumpAnd (FLAG_C,   0,                  offset);    break;
    case OP_JNO: _jumpAnd (0,        FLAG_OV,            offset);    break;
    case OP_JNE: _jumpAnd (0,        FLAG_EQ,            offset);    break;
    case OP_JEQ: _jumpAnd (FLAG_EQ,  0,                  offset);    break;

    case OP_SBZ:        _cruBitOutput (REGR(12), offset, 0);        break;
    case OP_SBO:        _cruBitOutput (REGR(12), offset, 1);        break;

    case OP_TB:
        // _st &= ~FLAG_EQ;
        // _st |= (cruBitGet (REGR(12), offset) ? FLAG_EQ : 0);
        _statusEqual (_cruBitGet (REGR(12), offset));
        break;

    default:
        _halt ("Bad jump opcode");
    }
}

void TMS9900::_executeDual1 (uint16_t opcode, uint16_t dReg, uint16_t sMode, uint16_t sReg)
{
    uint16_t sAddr;
    uint16_t sData;
    uint16_t dData;
    uint32_t u32;
    
    sAddr = _operandDecode (sMode, sReg, false);
    sData = _operandFetch (sMode, sReg, sAddr, false, true);

    switch (opcode)
    {
    case OP_COC:
        dData = REGR (dReg);
        _unasmPostExec ("&(R%d=%04X)=%04X", dReg, dData, sData & dData);
        _compareWord (sData & dData, sData);
        break;

    case OP_CZC:
        dData = REGR (dReg);
        _unasmPostExec ("&~(R%d=%04X)=%04X", dReg, dData, sData & ~dData);
        _compareWord (sData & ~dData, sData);
        break;

    case OP_XOR:
        dData = REGR (dReg);
        _unasmPostExec ("&~(R%d=%04X)=%04X", dReg, dData, sData ^ dData);
        dData ^= sData;
        REGW (dReg, dData);
        _compareWord (dData, 0);
        break;

    case OP_XOP:
        _xop (dReg, sData);
        break;

    case OP_MPY:
        dData = REGR (dReg);
        _unasmPostExec ("*(R%d=%04X)=%04X", dReg, dData, sData * dData);
        u32 = dData * sData;
        REGW(dReg, u32 >> 16);
        REGW(dReg+1, u32 & 0xFFFF);
        break;

    case OP_DIV:
        dData = REGR (dReg);
        if (sData <= dData)
        {
            _unasmPostExec ("<(%04X<%04X)->OVF", sData, dData);
            _statusOverflow (true);
        }
        else
        {
            _statusOverflow (false);
            u32 = REGR(dReg) << 16 | REGR(dReg+1);
            _unasmPostExec (",(%X/%X)=>%04X,%04X", u32, sData, u32 / sData, u32 % sData);
            REGW(dReg, u32 / sData);
            REGW(dReg+1, u32 % sData);
        }

        break;

    /*
     *  C R U
     */
    case OP_LDCR:
        if (dReg <= 8)
        {
            sData = _memReadB (sAddr);
            _statusParity (sData);
        }

        _cruMultiBitSet (REGR(12), sData, dReg);
        _debug ("LDCR R12=%x s=%x d=%x\n", REGR(12), sData, dReg);
        break;

    case OP_STCR:
        if (dReg <= 8)
        {
            sData = _cruMultiBitGet (REGR(12), dReg);
            _memWriteB(sAddr, sData);
            _statusParity (sData);
        }
        else
            _memWriteW(sAddr, _cruMultiBitGet (REGR(12), dReg));
        break;

    default:
        _halt ("Bad dual1 opcode");
    }
}

/*
 *  D U A L   O P E R A N D
 */
void TMS9900::_executeDual2 (uint16_t opcode, uint16_t dMode, uint16_t dReg,
                             uint16_t sMode, uint16_t sReg, bool isByte)
{
    bool doStore = true;
    uint16_t sAddr;
    uint16_t sData;
    uint16_t dAddr;
    uint16_t dData;
    uint32_t u32;

    sAddr = _operandDecode (sMode, sReg, isByte);
    sData = _operandFetch (sMode, sReg, sAddr, isByte, true);

    _unasmPostExec (",");
    dAddr = _operandDecode (dMode, dReg, isByte);
    /*  Don't fetch the contents of the destination if op is a MOV */
    dData = _operandFetch (dMode, dReg, dAddr, isByte,
                          (opcode != OP_MOV && opcode != OP_MOVB));

    switch (opcode)
    {
    case OP_SZC:
        dData &= ~sData;
        _unasmPostExec (":&~:%04X", dData);
        _compareWord (dData, 0);
        break;

    case OP_SZCB:
        dData &= ~sData;
        _unasmPostExec (":&~:%02X", dData);
        _compareByte (dData, 0);
        _statusParity (dData);
        break;

    case OP_S:
        u32 = (uint32_t) dData - sData;
        _statusOverflow ((sData & 0x8000) != (dData & 0x8000) &&
                        (u32 & 0x8000) != (dData & 0x8000));
        dData = u32 & 0xFFFF;
        u32 >>= 16;
        _unasmPostExec (":-:%04X", dData);

        /* 15-AUG-23 carry flag meaning is inverted for S, SB, DEC, DECT.  Where
         * is this documented ??????
         */
        // statusCarry (u32 != 0);
        _statusCarry (u32 == 0);
        _compareWord (dData, 0);
        break;

    case OP_SB:
        u32 = (uint32_t) dData - sData;
        _statusOverflow ((sData & 0x8000) != (dData & 0x8000) &&
                        (u32 & 0x8000) != (dData & 0x8000));
        dData = u32 & 0xFF;
        u32 >>= 8;
        _unasmPostExec (":-:%02X", dData);

        /* 15-AUG-23 carry flag meaning is inverted for S, SB, DEC, DECT.  Where
         * is this documented ??????
         */
        // statusCarry (u32 != 0);
        _statusCarry (u32 == 0);
        _compareByte (dData, 0);
        _statusParity (dData);
        break;

    case OP_C:
        _compareWord (sData, dData);
        _unasmPostExec (":==:");
        doStore = false;
        break;

    case OP_CB:
        _compareByte (sData, dData);
        _statusParity (sData);
        _unasmPostExec (":==:");
        doStore = false;
        break;

    case OP_MOV:
        dData = sData;
        _compareWord (dData, 0);
        break;

    case OP_MOVB:
        dData = sData;
        _statusParity (sData);
        _compareByte (dData, 0);
        break;

    case OP_SOC:
        dData |= sData;
        _unasmPostExec (":|:%04X", dData);
        _compareWord (dData, 0);
        break;

    case OP_SOCB:
        dData |= sData;
        _unasmPostExec (":|:%02X", dData);
        _compareByte (dData, 0);
        _statusParity (dData);
        break;

    case OP_A:
        u32 = (uint32_t) dData + sData;
        _statusOverflow ((sData & 0x8000) == (dData & 0x8000) &&
                        (u32 & 0x8000) != (dData & 0x8000));
        dData = u32 & 0xFFFF;
        _unasmPostExec (":+:%04X", dData);
        u32 >>= 16;

        _statusCarry (u32 != 0);
        _compareWord (dData, 0);
        break;

    case OP_AB:
        u32 = (uint32_t) dData + sData;
        _statusOverflow ((sData & 0x8000) == (dData & 0x8000) &&
                        (u32 & 0x8000) != (dData & 0x8000));
        dData = u32 & 0xFF;
        _unasmPostExec (":+:%02X", dData);
        u32 >>= 8;

        _statusCarry (u32 != 0);
        _compareByte (dData, 0);
        _statusParity (dData);
        break;

    default:
        _halt ("Bad dual2 opcode");
    }

    if (doStore)
    {
        if (isByte)
            _memWriteB (dAddr, dData);
        else
            _memWriteW (dAddr, dData);
    }
}

uint16_t TMS9900::decode (uint16_t data, uint16_t *type)
{
    OpGroup *o = &opGroup[data >> 10];
    *type = o->type;
    return (data & o->opMask);
}

void TMS9900::execute (uint16_t data)
{
    int sReg = 0;
    int sMode = 0;
    int dReg = 0;
    int dMode = 0;
    int8_t offset;
    bool isByte = false;
    uint16_t type;

    uint16_t opcode = decode (data, &type);

    _unasmPreExec (_pc, data, type, opcode);

    switch (type)
    {
    case OPTYPE_IMMED:
        sReg   =  data & 0x000F;
        _executeImmediate (opcode, sReg);
        break;

    case OPTYPE_SINGLE:
        sMode = (data & 0x0030) >> 4;
        sReg     =  data & 0x000F;
        _executeSingle (opcode, sMode, sReg);
        break;

    case OPTYPE_SHIFT:
        dReg = (data & 0x00F0) >> 4;
        sReg =  data & 0x000F;
        _executeShift (opcode, sReg, dReg);
        break;

    case OPTYPE_JUMP:
        offset = data & 0x00FF;
        _executeJump (opcode, offset);
        break;

    case OPTYPE_DUAL1:
        dReg     = (data & 0x03C0) >> 6;
        sMode = (data & 0x0030) >> 4;
        sReg     =  data & 0x000F;
        _executeDual1 (opcode, dReg, sMode, sReg);
        break;

    case OPTYPE_DUAL2:
        dMode = (data & 0x0C00) >> 10;
        dReg     = (data & 0x03C0) >> 6;
        sMode = (data & 0x0030) >> 4;
        sReg     =  data & 0x000F;
        isByte = (data & 0x1000) >> 12;
        _executeDual2 (opcode, dMode, dReg, sMode, sReg, isByte);
        break;

    default:
        _halt ("Bad optype");
    }

    _unasmEndLine ();

    int mask = _st & FLAG_MSK;
    int level = _interruptLevel (mask);

    if (level >= 0)
    {
        _debug ("interrupt level=%d st=%x\n", level, _st);
        _blwp (4 * level);

        /*  The ISR mask is automatically lowered to 1 less than the interrupt
         *  being serviced to ensure only higher priority interrupts can
         *  interrupt this ISR.
         */
        if (level > 0)
            mask = level - 1;
        else
            mask = 0;

        _st = (_st & ~FLAG_MSK) | mask;
    }
}

void TMS9900::showStatus(void)
{
    uint16_t i;

    _debug ("CPU\n");
    _debug ("===\n");
    _debug ("st=%04X\nwp=%04X\npc=%04X\n", _st, _wp, _pc);

    for (i = 0; i < 16; i++)
    {
        _debug ("R%02d: %04X ", i, REGR(i));
        if ((i + 1) % 4 == 0)
            _debug ("\n");
    }
}

void TMS9900::showStWord(void)
{
    _debug ("st=%04X %s int=%d)\n",
             _st, _outputStatus(), _st & 15);
}

void TMS9900::boot (void)
{
    _st = 0;
    _blwp (0x0);
}

void TMS9900::branch (uint16_t addr)
{
    _pc = addr;
}

