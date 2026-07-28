/*
 *  Signed division / modulo tests for 8-bit (signed char) and 16-bit (int)
 *  operands.  C99 6.5.5: division truncates toward zero, and
 *  (a/b)*b + a%b == a, so the remainder takes the sign of the dividend.
 *
 *  Values are loaded through volatile locals so the divisions cannot be
 *  constant-folded, and additionally through noinline helper functions to
 *  exercise the byte argument-passing / return convention.
 */

#include "tap.h"

typedef struct
{
    signed char a;
    signed char b;
    signed char div;    /* a / b */
    signed char mod;    /* a % b */
} case8_t;

static const case8_t c8[] =
{
    /* sign combinations, small values */
    {    7,    3,    2,    1 },
    {   -7,    3,   -2,   -1 },
    {    7,   -3,   -2,    1 },
    {   -7,   -3,    2,   -1 },

    /* discriminates signed vs unsigned interpretation of the dividend:
     * unsigned 255/2 would give 127, signed -1/2 must give 0 r -1 */
    {   -1,    2,    0,   -1 },
    {   -1,    1,   -1,    0 },

    /* full-range byte values */
    {  127,    7,   18,    1 },
    { -127,    7,  -18,   -1 },
    {  127,   -7,  -18,    1 },
    { -127,   -7,   18,   -1 },

    /* SCHAR_MIN edges (avoiding -128 / -1 which is undefined) */
    { -128,    1, -128,    0 },
    { -128,    2,  -64,    0 },
    { -128,   -2,   64,    0 },
    { -128,    7,  -18,   -2 },

    /* quotient zero */
    {    5,  127,    0,    5 },
    {   -5,  127,    0,   -5 },
    {    5, -127,    0,    5 },
    {   -5, -128,    0,   -5 },

    /* exact divisions */
    {  126,   -9,  -14,    0 },
    { -125,    5,  -25,    0 },
};

typedef struct
{
    int a;
    int b;
    int div;    /* a / b */
    int mod;    /* a % b */
} case16_t;

static const case16_t c16[] =
{
    /* sign combinations, small values */
    {      7,      3,      2,    1 },
    {     -7,      3,     -2,   -1 },
    {      7,     -3,     -2,    1 },
    {     -7,     -3,      2,   -1 },

    /* discriminates signed vs unsigned: unsigned 0xFFFF/2 = 32767 */
    {     -1,      2,      0,   -1 },

    /* two-byte values, all sign combinations */
    {    479,     57,      8,   23 },
    {   -479,     57,     -8,  -23 },
    {    479,    -57,     -8,   23 },
    {   -479,    -57,      8,  -23 },

    /* large dividends (top bit of magnitude set in the word) */
    {  30000,      7,   4285,    5 },
    { -30000,      7,  -4285,   -5 },
    {  30000,     -7,  -4285,    5 },
    { -30000,     -7,   4285,   -5 },

    /* INT_MIN edges (avoiding -32768 / -1 which is undefined) */
    { -32768,      1, -32768,    0 },
    { -32768,      2, -16384,    0 },
    { -32768,     -2,  16384,    0 },
    { -32768,      7,  -4681,   -1 },
    { -32768,  32767,     -1,   -1 },

    /* division by +-1 of extremes */
    {  32767,     -1, -32767,    0 },
    { -32767,     -1,  32767,    0 },

    /* quotient zero */
    {      5,  32767,      0,    5 },
    {     -5,  32767,      0,   -5 },
    {     -5, -32768,      0,   -5 },

    /* divisor larger than dividend in magnitude, both large */
    {  12345, -23456,      0, 12345 },
    { -12345,  23456,      0, -12345 },

    /* exact divisions */
    {  32130,    255,    126,    0 },
    { -32130,    255,   -126,    0 },
};

#define COUNT8  (sizeof (c8)  / sizeof (c8[0]))
#define COUNT16 (sizeof (c16) / sizeof (c16[0]))

/*  noinline helpers force the values through the QI argument / return
 *  convention (value in high byte of a word register) */
static signed char __attribute__ ((noinline)) sdiv8 (signed char a, signed char b)
{
    return a / b;
}

static signed char __attribute__ ((noinline)) smod8 (signed char a, signed char b)
{
    return a % b;
}

static int __attribute__ ((noinline)) sdiv16 (int a, int b)
{
    return a / b;
}

static int __attribute__ ((noinline)) smod16 (int a, int b)
{
    return a % b;
}

void t_i8_div (void)
{
    for (unsigned i = 0; i < COUNT8; i++)
    {
        volatile signed char va = c8[i].a;
        volatile signed char vb = c8[i].b;
        signed char q = va / vb;

        if (q != c8[i].div)
            test_printf ("i8 %d / %d == %d should be %d\n",
                         c8[i].a, c8[i].b, q, c8[i].div);

        test_execute (__func__, q == c8[i].div);
    }
}

void t_i8_mod (void)
{
    for (unsigned i = 0; i < COUNT8; i++)
    {
        volatile signed char va = c8[i].a;
        volatile signed char vb = c8[i].b;
        signed char r = va % vb;

        if (r != c8[i].mod)
            test_printf ("i8 %d %% %d == %d should be %d\n",
                         c8[i].a, c8[i].b, r, c8[i].mod);

        test_execute (__func__, r == c8[i].mod);
    }
}

void t_i8_div_call (void)
{
    for (unsigned i = 0; i < COUNT8; i++)
    {
        signed char q = sdiv8 (c8[i].a, c8[i].b);
        signed char r = smod8 (c8[i].a, c8[i].b);

        if (q != c8[i].div || r != c8[i].mod)
            test_printf ("i8 call %d /%% %d == %d,%d should be %d,%d\n",
                         c8[i].a, c8[i].b, q, r, c8[i].div, c8[i].mod);

        test_execute (__func__, q == c8[i].div && r == c8[i].mod);
    }
}

/*  8-bit operands with the result widened to int before use, so the
 *  division runs in HImode and the QI operands must be sign-extended */
void t_i8_div_widened (void)
{
    for (unsigned i = 0; i < COUNT8; i++)
    {
        volatile signed char va = c8[i].a;
        volatile signed char vb = c8[i].b;
        int q = va / vb;
        int r = va % vb;

        if (q != c8[i].div || r != c8[i].mod)
            test_printf ("i8 wide %d /%% %d == %d,%d should be %d,%d\n",
                         c8[i].a, c8[i].b, q, r, c8[i].div, c8[i].mod);

        test_execute (__func__, q == c8[i].div && r == c8[i].mod);
    }
}

void t_i16_div (void)
{
    for (unsigned i = 0; i < COUNT16; i++)
    {
        volatile int va = c16[i].a;
        volatile int vb = c16[i].b;
        int q = va / vb;

        if (q != c16[i].div)
            test_printf ("i16 %d / %d == %d should be %d\n",
                         c16[i].a, c16[i].b, q, c16[i].div);

        test_execute (__func__, q == c16[i].div);
    }
}

void t_i16_mod (void)
{
    for (unsigned i = 0; i < COUNT16; i++)
    {
        volatile int va = c16[i].a;
        volatile int vb = c16[i].b;
        int r = va % vb;

        if (r != c16[i].mod)
            test_printf ("i16 %d %% %d == %d should be %d\n",
                         c16[i].a, c16[i].b, r, c16[i].mod);

        test_execute (__func__, r == c16[i].mod);
    }
}

void t_i16_div_call (void)
{
    for (unsigned i = 0; i < COUNT16; i++)
    {
        int q = sdiv16 (c16[i].a, c16[i].b);
        int r = smod16 (c16[i].a, c16[i].b);

        if (q != c16[i].div || r != c16[i].mod)
            test_printf ("i16 call %d /%% %d == %d,%d should be %d,%d\n",
                         c16[i].a, c16[i].b, q, r, c16[i].div, c16[i].mod);

        test_execute (__func__, q == c16[i].div && r == c16[i].mod);
    }
}

TESTFUNC tests[] =
{
    t_i8_div,
    t_i8_mod,
    t_i8_div_call,
    t_i8_div_widened,
    t_i16_div,
    t_i16_mod,
    t_i16_div_call
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main (void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
