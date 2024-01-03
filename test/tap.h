#ifndef __TAP_H
#define __TAP_H

#undef USE_VDP

#if 1 // def __tms9900__
#ifdef USE_VDP
#include <vdp.h>
#endif

#include "temu.h"

/* Define 32 bit type so we have the same size values on host and target to
 * compare test results */
typedef long int32_t;
typedef unsigned long uint32_t;

#define printfd temu_printfd
#define printfs temu_printfs
#define printf printfs
#else
#include <stdio.h>
#include <string.h>

typedef int int32_t;
typedef unsigned int uint32_t;

#endif

#ifndef _DEBUG
#define dprintf(...)
#else
#define dprintf printfd
#endif


typedef void (*TESTFUNC) (void);

void test_execute (const char *name, int passed);
void test_report (void);
void test_run (TESTFUNC funcs[], int count);

#endif

