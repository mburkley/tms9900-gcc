#include "tap.h"

#include <stdarg.h>

void t_vdpwa (void)
{
    volatile char *ptr = 0x8800;

    *ptr = 0x12;
    *ptr=0x34;

    test_execute (__func__, 1);
}
TESTFUNC tests[] = 
{
    t_vdpwa
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
