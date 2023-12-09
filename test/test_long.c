#include "tap.h"

static void t_mul_short_int32 (void)
{
    int x =6;
    int y =7;
    int32_t z;
    z = x*y;

    test_execute (__func__, z==42);
}

static void t_int32_mul (void)
{
    int32_t x =6;
    int32_t y =7;
    int32_t z;
    z = x*y;

    test_execute (__func__, z==42);
}

static void t_int32_or()
{
    int32_t x = 0x56784321;
    int32_t y = 0x3a913c48;
    int32_t z = x | y;
    int32_t a = x | 0;
    int32_t b = x | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7ef97f69 && a==x && b==-1);
}

static void t_uint32_or()
{
    uint32_t x = 0x56784321;
    uint32_t y = 0x3a913c48;
    uint32_t z = x | y;
    uint32_t a = x | 0;
    uint32_t b = x | 0xffffffff;

    test_execute (__func__, z==0x7ef97f69 && a==x && b==0xffffffff);
}

static void t_int32_and()
{
    int32_t x = 0x56784321;
    int32_t y = 0x3a913c48;
    int32_t z = x & y;
    int32_t a = x & 0;
    int32_t b = x & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x12100000 && a==0 && b==x);
}

static void t_uint32_and()
{
    uint32_t x = 0x56784321;
    uint32_t y = 0x3a913c48;
    uint32_t z = x & y;
    uint32_t a = x & 0;
    uint32_t b = x & 0xffffffff;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x12100000 && a==0 && b==x);
}

static void t_int32_sla_lt16()
{
    int32_t x = 0x12341234;
    int32_t y;

    y = x << 3;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0x91a091a0);
}

void t_int32_sla_var()
{
    int32_t x = 0x12341234;
    int32_t y;

    int z=4;
    y = x<<z;
    dprintf("# y=%lx\n",y);
    test_execute (__func__, y== 0x23412340);
}

void t_int32_sla_var_zero()
{
    int32_t x = 0x12341234;
    int32_t y;

    int z=0;
    y = x<<z;
    dprintf("# y=%lx\n",y);
    test_execute (__func__, y== 0x12341234);
}

static void t_int32_sla_gt16()
{
    int32_t x = 0x12341234;
    int32_t y;

    y=x<<17;
    dprintf("# y=%lx\n",y);
    test_execute (__func__, y== 0x24680000);
}

static void t_int32_sla_16()
{
    int32_t x = 0x12341234;
    int32_t y;

    y=x<<16;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0x12340000);
}

static void t_int32_srl_lt16()
{
    uint32_t x = 0x87341234;
    uint32_t y;

    y = x >> 3;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0x10e68246);
}

static void t_int32_srl_var()
{
    uint32_t x = 0x87341234;
    uint32_t y;

    int z=4;
    y = x>>z;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0x08734123);
}

static void t_int32_srl_gt16()
{
    uint32_t x = 0x87341234;
    uint32_t y;

    y=x>>17;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0x0000439a);
}

static void t_int32_srl_16()
{
    uint32_t x = 0x87341234;
    uint32_t y;

    y=x>>16;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0x00008734);
}


static void t_int32_sra_lt16()
{
    int32_t x = 0x87341234;
    int32_t y;

    y = x >> 3;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0xf0e68246);
}

static void t_int32_sra_var()
{
    int32_t x = 0x87341234;
    int32_t y;

    int z=4;
    y = x>>z;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0xf8734123);
}

static void t_int32_sra_gt16()
{
    int32_t x = 0x87341234;
    int32_t y;

    y=x>>17;
    dprintf("# y=%x\n", y);
    test_execute (__func__, y== 0xffffc39a);
}

static void t_int32_sra_16()
{
    int32_t x = 0x87341234;
    int32_t y;

    y=x>>16;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== 0xffff8734);
}

TESTFUNC tests[] = 
{
    t_mul_short_int32,
    t_int32_mul,
    t_int32_or,
    t_uint32_or,
    t_int32_and,
    t_uint32_and,
    t_int32_sla_lt16,
    t_int32_sla_var,
    t_int32_sla_var_zero,
    t_int32_sla_gt16,
    t_int32_sla_16,
    t_int32_srl_lt16,
    t_int32_srl_var,
    t_int32_srl_gt16,
    t_int32_srl_16,
    t_int32_sra_lt16,
    t_int32_sra_var,
    t_int32_sra_gt16,
    t_int32_sra_16
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
