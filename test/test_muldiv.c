/*
 *  More comprehensive tests for 16-bit multiple and divide ops.  C99 standard
 *  https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1124.pdf section 6.5.5
 *  states (a/b)*b+a%b == a which implies result of a modulo operation with
 *  negative dividend is itself negative.
 */

#include "tap.h"

#define IFACTOR1 479 // random number to give 16 bit mult product
#define IFACTOR2 57
#define IMULRES (IFACTOR1*IFACTOR2)

int ix = IFACTOR1;
int iy = IFACTOR2;

void t_i16_mul (void)
{
    int z = (-ix)*iy;

    test_execute (__func__, z==-IMULRES);
}

void t_i16_div (void)
{
    int z = IMULRES / (-ix);

    test_execute (__func__, z==-iy);
}

void t_i16_mod (void)
{
    int z = -(IMULRES+7) % (ix);

    // printf("z=%d\n",z);
    test_execute (__func__, z==-7);
}

void t_u16_mul (void)
{
    unsigned x = ix;
    unsigned y = iy;
    unsigned z = x*y;

    test_execute (__func__, z==IMULRES);
}

#include "ltoa.c"

TESTFUNC tests[] = 
{
    t_i16_mul,
    t_i16_div,
    t_i16_mod,
    t_u16_mul
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
