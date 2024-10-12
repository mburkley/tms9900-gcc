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
#if 0
#include <ctype.h>
#include <arpa/inet.h>

#include "parse.h"
#include "files.h"
#endif
#include "types.h"
// #include "cpu.h"
// #include "mem.h"
#include "temu.h"

// Temu temu;

static void temu_printfd /*_regs*/ (uint16_t wp)
{
    char s[1000];
    char *sp = s;
    int c;
    int r[16];

    for (int i = 0; i < 16; i++)
        r[i] = memReadW (wp + i*2);

    while ((c = memReadB (r[1]++)) != 0)
       *sp++ = c;

    *sp = 0;
    // printf ("fmt=%s\n", s);
    printf (s, r[2], r[3], r[4], r[5], r[6]);
}

static void temu_printfd_stack (uint16_t wp)
{
    char s[1000];
    char *sp = s;
    int c;
    int r[16];

    int stack = memReadW (wp + 20);

    for (int i = 0; i < 16; i++)
        r[i] = memReadW (wp + i*2);

    while ((c = memReadB (r[1]++)) != 0)
       *sp++ = c;

    *sp = 0;
    // printf ("fmt=%s\n", s);
    printf (s, r[2], r[3], r[4], r[5], r[6]);
}

static void temu_printfs (uint16_t wp)
{
    int c;
    char s[16][100];

    for (int i = 0; i < 16; i++)
    {
        char *sp = s[i];
        int addr = memReadW (wp + i*2);

        while ((c = memReadB (addr++)) != 0)
           *sp++ = c;

        *sp = 0;
    }

    // printf ("fmt=%s\n", s);
    printf (s[1], s[2], s[3], s[4], s[5], s[6]);
}


char *Temu::getString (uint16_t addr)
{
    static char str[100];
    char *pos = str;
    uint8_t byte;

    // printf ("get str at %04x\n", addr);
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

    // printf ("wp=%04x name=%04x passed=%04x\n", getWP(), name, passed);
    if (passed)
    {
        printf ("ok %d - %s\n", _testsRun, getString (name));
        _testsPass++;
    }
    else
    {
        printf ("not ok %d - %s\n", _testsRun, getString (name));
    }
}

void Temu::test_report ()
{
    printf ("# %d of %d passed\n", _testsPass, _testsRun);
    exit (1);
}

void Temu::_xopHandler (uint8_t vector, uint16_t data)
{
    // printf ("Back to emu, vec=%d d=%04x\n", vector, data);

    if (vector != 15)
        return;

    static int testsRun;

    switch (data)
    {
    case 0: test_execute (); break;
    case 1: test_report (); break;
    }

    #if 0
    switch (opcode & 0xff)
    {
    case 1: temu_printfd(wp); break;
    case 2: temu_printfs(wp); break;
    case 3: exit(1); break;
    }
    #endif
}

static bool disasm = false;

int main (int argc, char *argv[])
{
    Temu temu;
    char c;

    while ((c = getopt(argc, argv, "d")) != -1)
    {
        switch (c)
        {
            case 'd' : disasm = true; break;
            // case 'b' : showBasic = true; break;
            default: printf ("Unknown option '%c'\n", c);
        }
    }

    if (argc - optind < 1)
    {
        printf ("\nBinary test tool\n\n"
                "usage: %s [-d] <bin-file>\n"
                "\t where -d=disassemble\n\n", argv[0]);
        return 1;
    }

    memLoad (argv[optind], 0x6000, 0);

    // cpuHandleBadOpcode (opcodeTrap);

    // if (disasm)
    //     outputLevel = LVL_CPU|LVL_UNASM;

    //  Hardcoded to cartridge _start for now
    temu.branch(0x6026);
    temu.run();

    while (temu.running())
    // for (int i =0; i < 2; i++)
    {
        uint16_t opcode = temu.fetch ();
        temu.execute(opcode);
        // std::cout << "out";
        if (disasm)
        std::cout << temu.unasm.getOutput();
        temu.unasm.clearOutput();
    }

    return 0;
}
