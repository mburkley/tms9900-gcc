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
 *  This is a very minimal TI994A / TMS9900 emulator for unit testing the gcc compiler.
 *
 *  It has no support for graphics, sound, grom or other devices.  There is a simple 
 *  flat 64KB memory space.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "types.h"
#include "harness.h"
#include "../xop.h"

char *Harness::getString (uint16_t addr)
{
    static char str[100];
    char *pos = str;
    uint8_t byte;

    do
    {
        byte = memReadB (addr++);
        *pos++ = byte;
    }
    while (byte != 0);

    return str;
}

void Harness::test_execute ()
{
    _testsRun++;

    int name = memReadW(getWP()+2); // R1
    int passed = memReadW(getWP()+4); // R2

    if (passed)
    {
        printf ("ok %d - %s\n", _testsRun, getString (name));
        _testsPass++;
    }
    else
    {
        printf ("not ok %d - %s\n", _testsRun, getString (name));

        std::cout << getDisassembly();
    }

    clearDisassembly();
}

void Harness::test_start ()
{
    int count = memReadW(getWP()+2); // R1
    printf ("1..%d\n", count);
}

void Harness::test_report ()
{
    printf ("# %d of %d passed\n", _testsPass, _testsRun);
    stop ();
}

// Host printf function - NOTE : numeric values only
// Caller passed fmt string and params on stack

void Harness::test_printf()
{
    int c;
    int s[10];
    int stack;

    stack = memReadW (getWP() + 20); // R10

    for (int i = 0; i < 10; i++)
        s[i] = memReadW (stack + (i+1)*2);

    char *fmt = getString (s[0]);

    printf (fmt, s[1], s[2], s[3],
            s[4], s[5], s[6],
            s[7], s[8], s[9]);
}

void Harness::_xopHandler (uint8_t vector, uint16_t data)
{
    if (vector != 15)
        return;

    static int testsRun;

    switch (data)
    {
    case XOP_EXECUTE: test_execute (); break;
    case XOP_START: test_start (); break;
    case XOP_REPORT: test_report (); break;
    case XOP_PRINT: test_printf (); break;
    default: std::cerr <<"Unknown XOP " << data << std::endl; exit (1); break;
    }
}

int main (int argc, char *argv[])
{
    Harness harness;
    char c;
    bool disasmAll = false;
    bool disasmFails = false;

    while ((c = getopt(argc, argv, "df")) != -1)
    {
        switch (c)
        {
            case 'd' : disasmAll = true; break;
            case 'f' : disasmFails = true; break;
            default: printf ("Unknown option '%c'\n", c);
        }
    }

    if (argc - optind < 1)
    {
        printf ("\nBinary test tool\n\n"
                "usage: %s [-d] [-f] <bin-file>\n"
                "\t where -d=disassemble all tests\n"
                "\t       -f=disassemble only failed tests\n", argv[0]);
        return 1;
    }

    memLoad (argv[optind], 0x6000, 0);

    //  Hardcoded to cartridge _start for now
    harness.branch(0x6026);
    harness.run();

    while (harness.running())
    {
        uint16_t opcode = harness.fetch ();
        harness.execute(opcode);
        if (disasmAll)
            std::cout << harness.unasm.getOutput();
        else if (disasmFails)
            harness.captureDisassembly();
        harness.unasm.clearOutput();
    }

    return harness.failCount();
}
