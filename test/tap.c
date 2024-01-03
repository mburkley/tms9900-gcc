
#include "tap.h"

static int testsRun = 0;
static int testsPass = 0;

void test_execute (const char *name, int passed)
{
    testsRun++;

    if (passed)
    {
        printfd ("ok %d - ", testsRun);
        testsPass++;
    }
    else
        printfd ("not ok %d - ", testsRun);

    printfs ("%s\n", name);
}

void test_report (void)
{
    printfd ("# %d of %d passed\n", testsPass, testsRun);
}

void test_run (TESTFUNC tests[], int count)
{
// #ifdef __tms9900__
#ifdef USE_VDP
    set_graphics(0);
    charset();
#endif
    printfd ("1..%d\n", count);

    int i;
    for (i = 0; i < count; i++)
        tests[i]();
        
    test_report ();

// #ifdef __tms9900__
#ifdef USE_VDP
    while(1);
#else
    temu_quit();
#endif
}

