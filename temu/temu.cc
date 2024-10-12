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
#include "temu.h"

char *Temu::getString (uint16_t addr)
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

void Temu::test_execute ()
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

void Temu::test_report ()
{
    printf ("# %d of %d passed\n", _testsPass, _testsRun);
    exit (1);
}

// Host printf function - NOTE : numeric values only
void Temu::test_printf()
{
    int c;
    int r[16];

    for (int i = 0; i < 16; i++)
        r[i] = memReadW (getWP() + i*2);

    char *fmt = getString (r[1]);

    printf (fmt, r[2], r[3], r[4], r[5], r[6]);
}

void Temu::_xopHandler (uint8_t vector, uint16_t data)
{
    if (vector != 15)
        return;

    static int testsRun;

    switch (data)
    {
    case 0: test_execute (); break;
    case 1: test_report (); break;
    case 2: test_printf (); break;
    }
}

int main (int argc, char *argv[])
{
    Temu temu;
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
    temu.branch(0x6026);
    temu.run();

    while (temu.running())
    {
        uint16_t opcode = temu.fetch ();
        temu.execute(opcode);
        if (disasmAll)
            std::cout << temu.unasm.getOutput();
        else if (disasmFails)
            temu.captureDisassembly();
        temu.unasm.clearOutput();
    }

    return 0;
}
