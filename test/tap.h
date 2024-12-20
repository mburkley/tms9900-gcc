/*
 *  There are three possible build environments:
 *
 *  1) TI99/4A cartridge.  Uses libti99
        __tms9900__ defined
 *  2) Emulated TI99.  Uses XOP traps
        EMUL_TEST=1 and __tms9900__ defined
 *  3) Host.  Assumes linux x86-64
        Neither __tms9900__ nor EMUL_TEST defined
 */
#ifndef __TAP_H
#define __TAP_H

#include "xop.h"

#ifdef __tms9900__

#ifndef EMUL_TEST
#include <vdp.h>
#define USE_VDP 1
#endif

/* Define 32 bit type so we have the same size values on host and target to
 * compare test results */
typedef long int32_t;
typedef unsigned long uint32_t;

#else // tms9900
#include <stdio.h>
#include <string.h>

typedef int int32_t;
typedef unsigned int uint32_t;

#endif // tms9900

#ifndef _DEBUG
#define dprintf(...)
#else
#ifdef EMUL_TEST
#define dprintf test_printf
#else
#define dprintf printf
#endif
#endif

typedef void (*TESTFUNC) (void);

void test_dummy_void (void);
void test_dummy_int (int x);
void test_execute (const char *name, int passed);
void test_report (void);
void test_printf (char *fmt, ...);
void test_run (TESTFUNC funcs[], int count);

#endif

