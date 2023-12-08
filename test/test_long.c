#include "tap.h"

static void t_mul_short_long (void)
{
    int x =6;
    int y =7;
    long z;
    z = x*y;

    test_execute (__func__, z==42);
}

static void t_s_long_or()
{
    // Disabled until libgcc is working
    #if 0
    long x = 0x4321;
    long y = 0x3c48;
    long z = x | y;
    long a = x | 0;
    long b = x | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7f69 && a==x && b==-1);
    #endif
}

static void t_us_long_or()
{
    // Disabled until libgcc is working
    #if 0
    unsigned long x = 0x4321;
    unsigned long y = 0x3c48;
    unsigned long z = x | y;
    unsigned long a = x | 0;
    unsigned long b = x | 0xff;

    test_execute (__func__, z==0x7f69 && a==x && b==0xff);
    #endif
}

static void t_s_long_and()
{
    // Disabled until libgcc is wandking
    #if 0
    long x = 0x4321;
    long y = 0x3c48;
    long z = x & y;
    long a = x & 0;
    long b = x & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7f69 && a==0 && b==x);
    #endif
}

static void t_us_long_and()
{
    // Disabled until libgcc is working
    #if 0
    unsigned long x = 0x4321;
    unsigned long y = 0x3c48;
    unsigned long z = x & y;
    unsigned long a = x & 0;
    unsigned long b = x & 0xffff;

    test_execute (__func__, z==0x7f69 && a==0 && b==x);
    #endif
}

TESTFUNC tests[] = 
{
    t_mul_short_long,
    t_s_long_or,
    t_us_long_or,
    t_s_long_and,
    t_us_long_and
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
