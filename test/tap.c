#ifdef __tms9900__
#ifndef EMUL_TEST
#include <vdp.h>
#endif
#else
#include <stdio.h>
#endif

#include "tap.h"

static int testsRun = 0;
static int testsPass = 0;

/*  Dummy functions that may be called by test functions */
void test_dummy_void (void)
{
}

void test_dummy_int (int x)
{
    x=x;
}

#ifdef EMUL_TEST
void test_execute (const char *name, int passed)
{
    __asm__(
            "li r0,0\n\t"
            "xop r0,15\n\t");
}

void test_report (void)
{
    __asm__(
            "li r0,1\n\t"
            "xop r0,15\n\t");
}

void test_printf (void)
{
    __asm__(
            "li r0,2\n\t"
            "xop r0,15\n\t");
}

#else
void test_execute (const char *name, int passed)
{
    testsRun++;

    if (passed)
    {
        printf ("ok %d - %s\n", testsRun, name);
        testsPass++;
    }
    else
    {
        printf ("not ok %d - %s\n", testsRun, name);
    }

}

void test_report (void)
{
    printf ("# %d of %d passed\n", testsPass, testsRun);
}
#endif
void test_run (TESTFUNC tests[], int count)
{
#ifdef USE_VDP
    set_graphics(0);
    charset();
    printf ("1..%d\n", count);
#endif

    int i;
    for (i = 0; i < count; i++)
        tests[i]();
        
    test_report ();

#ifdef USE_VDP
    while(1);
#endif
}

