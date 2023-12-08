#ifdef __tms9900__
#include <vdp.h>
#else
#include <stdio.h>
#endif

#include "tap.h"

static int testsRun = 0;
static int testsPass = 0;

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

void test_run (TESTFUNC tests[], int count)
{
#ifdef __tms9900__
    set_graphics(0);
    charset();
#endif
    printf ("1..%d\n", count);

    int i;
    for (i = 0; i < count; i++)
        tests[i]();
        
    test_report ();

#ifdef __tms9900__
    while(1);
#endif
}

