#include "tap.h"

static int func2()
{
    return -1;
}

static int func()
{
    return func2() < 0;
}

static void t_sign_ret()
{
    printf("# ret=%d\n", func());
    test_execute (__func__, func () != 0);
}

TESTFUNC tests[] = 
{
    t_sign_ret
};

int main (void)
{
    test_run (tests, 1);
}

