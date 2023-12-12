#ifndef __TAP_H
#define __TAP_H

#ifdef __tms9900__
#include <vdp.h>

/* Define 32 bit type so we have the same size values on host and target to
 * compare test results */
typedef long int32_t;
typedef unsigned long uint32_t;

#else
#include <stdio.h>
#include <string.h>

typedef int int32_t;
typedef unsigned int uint32_t;

#endif

#ifndef _DEBUG
#define dprintf(...)
#else
#define dprintf printf
#endif


typedef void (*TESTFUNC) (void);

void test_execute (const char *name, int passed);
void test_report (void);
void test_run (TESTFUNC funcs[], int count);

#endif

