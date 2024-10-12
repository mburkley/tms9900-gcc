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

#ifndef __MEM_H
#define __MEM_H

#include "cpu.h"

#define ROM_FILE_SIZE   0x2000

#define BANKS_DEVICE    16
#define BANKS_CARTRIDGE 64 // Up to 512KiB banked cartridge ROM

uint16_t memRead(uint16_t addr, int size);
void memWrite(uint16_t addr, uint16_t data, int size);
uint16_t memReadW(uint16_t addr);
void memWriteW(uint16_t addr, uint16_t data);
uint16_t memReadB(uint16_t addr);
void memWriteB(uint16_t addr, uint8_t data);
int memLoad (char *file, uint16_t addr, int bank);
void memMapFile (const char *name, uint16_t addr, uint16_t size);
void memCopy (uint8_t *copy, uint16_t addr, int bank);
void memPrintScratchMemory (uint16_t addr, int len);
bool memDeviceRomSelect (int index, uint8_t state);

#endif

