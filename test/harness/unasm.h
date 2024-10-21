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

#ifndef __UNASM_H
#define __UNASM_H

#include <string>
#include <iostream>
#include <cstdlib>

#include "types.h"

class Unasm
{
public:
    Unasm();
    void readCodeComments (const char *textFile);
    uint16_t preExec (uint16_t pc, uint16_t data, uint16_t type, uint16_t opcode);
    void vPostExec (const char *fmt, va_list ap);
    void postExec (const char *fmt, ...);
    void endLine (void);
    std::string getOutput() { return _output; }
    void clearOutput () { _output = ""; }
    void outputUncovered (bool state);
private:
    std::string _output;
    std::string _execOutput;
    bool _covered[0x8000];
    bool _outputUncovered;
    bool _skipCurrent;
    const char *_codeComments[0x10000];
    const char *_currComment;

    std::string _align (const char *s, int len);
    virtual void _halt (const char *msg) { std::cerr << msg; exit (1); }
    const char *_parseComment (char type, int *len);
    std::string _printOper (uint16_t mode, uint16_t reg, uint16_t *pc);
    void _twoOp (uint16_t opCode, uint16_t *pc, uint16_t sMode, uint16_t sReg,
                        uint16_t dMode, uint16_t dReg);
    void _oneOp (uint16_t opCode, uint16_t *pc, uint16_t sMode, uint16_t sReg);
    void _immed (uint16_t opCode, uint16_t *pc, uint16_t sReg);
    void _jump (uint16_t opCode, uint16_t pc, int8_t offset);
};

#endif

