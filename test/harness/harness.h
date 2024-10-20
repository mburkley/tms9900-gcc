
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

#ifndef __HARNESS_H
#define __HARNESS_H

#include <stdarg.h>

#include "cpu.h"
#include "mem.h"
#include "unasm.h"

class Harness:public TMS9900
{
public:
    Harness() { _runFlag = false; _testsRun = _testsPass = 0; }
    Unasm unasm;
    void run () { _runFlag = true; }
    void stop () { _runFlag = false; }
    bool running () { return _runFlag; }
    void captureDisassembly () { _testDisassembly += unasm.getOutput(); }
    std::string getDisassembly () { return _testDisassembly; }
    void clearDisassembly() { _testDisassembly = ""; }
    int failCount () { return _testsRun - _testsPass; }

private:
    bool _runFlag;
    std::string _testDisassembly;
    int _testsRun;
    int _testsPass;
    char *getString (uint16_t addr);
    void test_execute ();
    void test_start ();
    void test_report ();
    void test_printf ();
    void _xopHandler (uint8_t vector, uint16_t data);
    uint16_t _memReadW (uint16_t addr) { return memReadW (addr); }
    uint8_t _memReadB (uint16_t addr) { return memReadB (addr); }
    void _memWriteW (uint16_t addr, uint16_t data) { memWriteW (addr, data); }
    void _memWriteB (uint16_t addr, uint8_t data) { memWriteB (addr, data); }

    // TODO no interrupts for now
    int _interruptLevel (int mask) { return -1; }

    void _halt (const char *s) { std::cerr << s; exit(1); }

    void _xop (uint8_t vector, uint16_t data) { _xopHandler (vector, data); }

    void _unasmPostExec (const char *fmt, ...)
    {
        va_list ap;

        va_start (ap, fmt);
        unasm.vPostExec (fmt, ap);
        va_end (ap);
    }

    uint16_t _unasmPreExec (uint16_t pc, uint16_t data, uint16_t type, uint16_t opcode)
    { return unasm.preExec (pc, data, type, opcode ); }
    void _unasmEndLine (void) { unasm.endLine(); }
};

#endif

