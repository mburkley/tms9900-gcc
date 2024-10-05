#include "tap.h"

char a[4];

void t_array (void)
{
    int i;
    for (i = 0; i < 4; i++)
        ++a[i];

    test_execute (__func__, a[2]==1);
}

TESTFUNC tests[] = 
{
    t_array
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
