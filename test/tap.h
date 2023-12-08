#ifndef __TAP_H
#define __TAP_H

#ifdef __tms9900__
#include <vdp.h>
#else
#include <stdio.h>
#include <string.h>
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

