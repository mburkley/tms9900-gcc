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
 *  unasm.c  - TMS9900 disassembler.
 *
 *  Pre and post operation execution for disassembly
 *  at run time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "types.h"
#include "cpu.h"
#include "mem.h"
#include "parse.h"
#include "unasm.h"

Unasm::Unasm()
{
    memset (_covered, 0, 0x8000);
    _outputUncovered = false;
    _skipCurrent = false;
}

std::string Unasm::_align (const char *msg, int width)
{
    char s[width+1];

    sprintf (s, "%-*.*s", width, width, msg);
    return std::string (s);
}

// TODO move commenting out of unasm - not needed for harness
const char *Unasm::_parseComment (char type, int *len)
{
    const char *next;
    const char *ret;

    if (_currComment == NULL)
        return NULL;

    if (*_currComment != '@')
        _halt ("invalid comment");

    if (*(_currComment+1) != type)
        return NULL;

    next = strchr ((char*)_currComment+2, '@');

    if (next)
        *len = next - _currComment - 2;
    else
        *len = strlen (_currComment) - 2;

    ret = _currComment + 2;

    _currComment = next;
    return ret;
}

std::string Unasm::_printOper (uint16_t mode, uint16_t reg, uint16_t *pc)
{
    char result[20];
    uint16_t arg;

    switch (mode)
    {
    case AMODE_NORMAL:
        sprintf (result, "%d", reg);
        break;

    case AMODE_INDIR:
        sprintf (result, "*%d", reg);
        break;

    case AMODE_SYM:
        arg = memReadW (*pc);
        (*pc) += 2;

        if (reg)
        {
            sprintf (result, "@>%04X[%d]", arg, reg);
        }
        else
        {
            sprintf (result, "@>%04X", arg);
        }

        break;

    case AMODE_INDIRINC:
        sprintf (result, "*%d+", reg);
        break;
    }

    return std::string(result);
}

void Unasm::_twoOp (uint16_t opCode, uint16_t *pc, uint16_t sMode, uint16_t sReg,
                        uint16_t dMode, uint16_t dReg)
{
    const char *name = "****";

    switch (opCode)
    {
    case OP_SZC:  name = "SZC ";  break;
    case OP_SZCB: name = "SZCB";  break;
    case OP_S:    name = "S   ";  break;
    case OP_SB:   name = "SB  ";  break;
    case OP_C:    name = "C   ";  break;
    case OP_CB:   name = "CB  ";  break;
    case OP_A:    name = "A   ";  break;
    case OP_AB:   name = "AB  ";  break;
    case OP_MOV:  name = "MOV ";  break;
    case OP_MOVB: name = "MOVB";  break;
    case OP_SOC:  name = "SOC ";  break;
    case OP_SOCB: name = "SOCB";  break;
    case OP_SRA:  name = "SRA ";  break;
    case OP_SRC:  name = "SRC ";  break;
    case OP_SRL:  name = "SRL ";  break;
    case OP_SLA:  name = "SLA ";  break;
    case OP_COC:  name = "COC ";  break;
    case OP_CZC:  name = "CZC ";  break;
    case OP_XOR:  name = "XOR ";  break;
    case OP_XOP:  name = "XOP ";  break;
    case OP_LDCR: name = "LDCR";  break;
    case OP_STCR: name = "STCR";  break;
    case OP_MPY:  name = "MPY ";  break;
    case OP_DIV:  name = "DIV ";  break;

    }

    char out[31];

    sprintf (out, "%-4s  %s,%s",
             name,
             _printOper (sMode, sReg, pc).c_str(),
             _printOper (dMode, dReg, pc).c_str());

    _output += _align (out, 30);
}

void Unasm::_oneOp (uint16_t opCode, uint16_t *pc, uint16_t sMode, uint16_t sReg)
{
    const char *name = "****";

    switch (opCode)
    {
    case OP_BLWP: name = "BLWP"; break;
    case OP_B:    name = "B";   break;
    case OP_X:    name = "X";   break;
    case OP_CLR:  name = "CLR"; break;
    case OP_NEG:  name = "NEG"; break;
    case OP_INV:  name = "INV"; break;
    case OP_INC:  name = "INC"; break;
    case OP_INCT: name = "INCT";    break;
    case OP_DEC:  name = "DEC"; break;
    case OP_DECT: name = "DECT";    break;
    case OP_BL:   name = "BL";  break;
    case OP_SWPB: name = "SWPB";    break;
    case OP_SETO: name = "SETO";    break;
    case OP_ABS:  name = "ABS"; break;
    }

    char out[31];
    sprintf (out, "%-4s  %s",
             name,
             _printOper (sMode, sReg, pc).c_str());

    _output += _align (out, 30);
}

void Unasm::_immed (uint16_t opCode, uint16_t *pc, uint16_t sReg)
{
    char out[31];
    const char *name = "****";
    bool showReg = true;
    bool showOper = true;
    bool showData = true;

    switch (opCode)
    {
    case OP_LI: name="LI"; break;
    case OP_AI: name="AI"; break;
    case OP_ANDI: name="ANDI"; break;
    case OP_ORI: name="ORI"; break;
    case OP_CI: name="CI"; break;
    case OP_STWP: name="STWP"; showData = false; break;
    case OP_STST: name="STST"; showData = false; break;
    case OP_LWPI: name="LWPI"; showReg = false; break;
    case OP_LIMI: name="LIMI"; showReg = false; break;
    case OP_RTWP: name="RTWP"; showOper = false; break;
    default: showOper = false; break;
    }

    if (showOper)
    {
        if (showData)
        {

            if (showReg)
            {
                sprintf (out, "%-4s  %d,>%04X",
                         name,
                         sReg,
                         memReadW(*pc));
            }
            else
            {
                sprintf (out, "%-4s  >%04X",
                         name,
                         memReadW(*pc));
            }
            (*pc) += 2;
        }
        else
        {
            sprintf (out, "%-4s  %d",
                     name,
                     sReg);
        }
    }
    else
        sprintf (out, "%-4s",
                 name);

    _output += _align (out, 30);
}

void Unasm::_jump (uint16_t opCode, uint16_t pc, int8_t offset)
{
    char out[31];
    const char *name = "****";
    int pcOffset = 1;

    switch (opCode)
    {
    case OP_JMP: name="JMP"; break;
    case OP_JLT: name="JLT"; break;
    case OP_JLE: name="JLE"; break;
    case OP_JEQ: name="JEQ"; break;
    case OP_JHE: name="JHE"; break;
    case OP_JGT: name="JGT"; break;
    case OP_JNE: name="JNE"; break;
    case OP_JNC: name="JNC"; break;
    case OP_JOC: name="JOC"; break;
    case OP_JNO: name="JNO"; break;
    case OP_JL: name="JL"; break;
    case OP_JH: name="JH"; break;
    case OP_JOP: name="JOP"; break;
    case OP_SBO: name="SBO"; pcOffset = 0; break;
    case OP_SBZ: name="SBZ"; pcOffset = 0; break;
    case OP_TB: name="TB"; pcOffset = 0; break;
    }

    if (pcOffset)
    {
        sprintf (out, "%-4s  >%04X",
                name,
                pc + (offset << 1));
    }
    else
    {
        sprintf (out, "%-4s  %d",
                name,
                offset);
    }

    _output += _align (out, 30);
}

/*  Pre execution disassembly.  Returns number of words consumed */
uint16_t Unasm::preExec (uint16_t pc, uint16_t data, uint16_t type, uint16_t opcode)
{
    const char *comment;
    int len;
    int sReg = 0;
    int dReg = 0;
    int sMode = 0;
    int dMode = 0;
    uint16_t pcStart = pc;

    if (_covered[pc>>1])
    {
        _skipCurrent = true;
        return 0;
    }

    if (_outputUncovered)
    {
        _covered[pc>>1] = true;
        _skipCurrent = false;
    }

    _currComment = _codeComments[pc-2];

    while ((comment = _parseComment ('-', &len)) != NULL)
    {
        _output += ";";
        _output += _align (comment, len);
        _output += "\n";
    }

    char addr[11];
    sprintf (addr, "%04X:%04X ", pc-2, data);
    _output += addr;

    switch (type)
    {
    case OPTYPE_IMMED:
        sReg   =  data & 0x000F;
        _immed (opcode, &pc, sReg);
        break;

    case OPTYPE_SINGLE:
        sMode = (data & 0x0030) >> 4;
        sReg     =  data & 0x000F;
        _oneOp (opcode, &pc, sMode, sReg);
        break;

    case OPTYPE_SHIFT:
        dReg = (data & 0x00F0) >> 4;
        sReg =  data & 0x000F;
        _twoOp (opcode, &pc, 0, sReg, 0, dReg);
        break;

    case OPTYPE_JUMP:
        sReg = data & 0x00FF;
        _jump (opcode, pc, sReg);
        break;

    case OPTYPE_DUAL1:
        dReg     = (data & 0x03C0) >> 6;
        sMode = (data & 0x0030) >> 4;
        sReg     =  data & 0x000F;
        _twoOp (opcode, &pc, sMode, sReg, 0, dReg);
        break;

    case OPTYPE_DUAL2:
        dMode = (data & 0x0C00) >> 10;
        dReg     = (data & 0x03C0) >> 6;
        sMode = (data & 0x0030) >> 4;
        sReg     =  data & 0x000F;
        _twoOp (opcode, &pc, sMode, sReg, dMode, dReg);
        break;
    }

    return pc - pcStart;
}

void Unasm::vPostExec (const char *fmt, va_list ap)
{
    if (_skipCurrent)
        return;

    char out[200];
    vsprintf (out, fmt, ap);
    _execOutput += out;
}

void Unasm::postExec (const char *fmt, ...)
{
    va_list ap;

    va_start (ap, fmt);
    vPostExec (fmt, ap);
    va_end (ap);
}

void Unasm::endLine (void)
{
    const char *comment;
    int len;
    bool anyComment = false;

    if (_skipCurrent)
        return;

    _output += _align (_execOutput.c_str(), 40);
    _execOutput = "";

    while ((comment = _parseComment ('+', &len)) != NULL)
    {
        anyComment = true;
        _output += _align (comment, len);
        _output += "\n";
    }

    if (!anyComment)
        _output += "\n";
}

/*  Reads comments in from a file and displays them after an instruction that
 *  has been executed.
 *
 *  NOTE : At present, the file has no way to select ROM banks for even
 *  cartridges.  Different files should be used for different cartridges.  At
 *  the moment, unasm.txt contains comments for console ROM, disk DSR and
 *  extended basic (a mix of C and D).
 */
void Unasm::readCodeComments (const char *textFile)
{
    FILE *fp;
    char s[2048];

    if ((fp = fopen (textFile, "r")) == NULL)
    {
        printf ("** Failed to open %s\n", textFile);
        return;
    }

    while (!feof (fp))
    {
        if (!fgets (s, sizeof s, fp))
            continue;

        uint16_t ix = strtoul (s, NULL, 16);
        if (_codeComments[ix])
        {
            printf ("Already have text for %x\n", ix);
        }
        else
        {
            if (s[strlen(s)-1]=='\n')
                s[strlen(s)-1] = 0;

            _codeComments[ix] = strdup (&s[5]);
        }
    }

    fclose (fp);
}

void Unasm::outputUncovered (bool state)
{
    _outputUncovered = state;
}

