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
 *  Simple flat 64KB byte array and simple memory access methods
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "mem.h"

uint8_t memory[64 * 1024];

uint16_t memReadW (uint16_t addr)
{
    // printf ("[MR:%04x]", addr);
    addr &= ~1; // Always round word reads to a word boundary
    return (memory[addr] << 8) | memory[addr+1];
}
uint16_t memReadB (uint16_t addr)
{
    return memory[addr];
}

void memWriteW (uint16_t addr, uint16_t value)
{
    addr &= ~1; // Always round word reads to a word boundary
    memory[addr++] = value >> 8;
    memory[addr] = value & 0xFF;
}

void memWriteB (uint16_t addr, uint8_t value)
{
    memory[addr] = value;
}

/*  Load a file into memory. */
int memLoad (char *file, uint16_t addr, int bank)
{
    FILE *fp;

    if ((fp = fopen (file, "rb")) == NULL)
    {
        printf ("can't open ROM bin file '%s'\n", file);
        exit (1);
    }

    fseek (fp, 0, SEEK_END);
    int len = ftell (fp);
    fseek (fp, 0, SEEK_SET);

    printf("%s %s %x %x\n", __func__, file, addr, len);

    int got;

    if ((got=fread (&memory[addr], sizeof (uint8_t), len, fp)) != len)
    {
        printf ("%s expected len=%04X, got=%04X\n", file, len, got);
        perror("fread");
        exit (1);
    }

    fclose (fp);
    return len;
}

