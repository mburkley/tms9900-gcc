/*  Quick sanity check that struct bitfields work: set/get, signed fields,
 *  a field straddling a 16-bit word boundary, read-modify-write, and
 *  char-container fields.  Not comprehensive. */

#include "tap.h"

struct bf
{
    unsigned a : 3;
    unsigned b : 5;
    unsigned c : 7;
    signed   d : 4;   /* straddles the first 16-bit word */
    unsigned e : 13;
};

struct cbf
{
    unsigned char x : 2;
    unsigned char y : 3;
    signed char   z : 3;
};

static struct bf g = { 5, 21, 100, -3, 4321 };

static unsigned __attribute__ ((noinline)) get_b (struct bf *p) { return p->b; }
static int      __attribute__ ((noinline)) get_d (struct bf *p) { return p->d; }
static void     __attribute__ ((noinline)) set_d (struct bf *p, int v) { p->d = v; }

void t_bf_init (void)
{
    /* static initializer laid down by the compiler, read back at runtime */
    test_execute (__func__, g.a == 5);
    test_execute (__func__, g.b == 21);
    test_execute (__func__, g.c == 100);
    test_execute (__func__, g.d == -3);
    test_execute (__func__, g.e == 4321);
}

void t_bf_setget (void)
{
    volatile struct bf s;

    s.a = 7; s.b = 0; s.c = 127; s.d = -8; s.e = 8191;
    test_execute (__func__, s.a == 7);
    test_execute (__func__, s.b == 0);
    test_execute (__func__, s.c == 127);
    test_execute (__func__, s.d == -8);
    test_execute (__func__, s.e == 8191);

    /* neighbours untouched after single-field writes */
    s.b = 31;
    s.d = 7;
    test_execute (__func__, s.a == 7 && s.c == 127 && s.e == 8191);
    test_execute (__func__, s.b == 31);
    test_execute (__func__, s.d == 7);

    /* wraparound on overflow */
    s.a = 9;            /* 3 bits: 9 & 7 == 1 */
    test_execute (__func__, s.a == 1);
}

void t_bf_rmw (void)
{
    struct bf s = { 1, 2, 3, 1, 5 };

    s.b += 10;
    s.d -= 4;
    s.c ^= 0x55;
    test_execute (__func__, s.b == 12);
    test_execute (__func__, s.d == -3);
    test_execute (__func__, s.c == (3 ^ 0x55));
    test_execute (__func__, s.a == 1 && s.e == 5);
}

void t_bf_calls (void)
{
    set_d (&g, -4);
    test_execute (__func__, get_d (&g) == -4);
    test_execute (__func__, get_b (&g) == 21);
    set_d (&g, 3);
    test_execute (__func__, get_d (&g) == 3);
    test_execute (__func__, g.c == 100 && g.e == 4321);
}

void t_bf_char (void)
{
    volatile struct cbf s;

    s.x = 3; s.y = 5; s.z = -2;
    test_execute (__func__, s.x == 3);
    test_execute (__func__, s.y == 5);
    test_execute (__func__, s.z == -2);
    // Claude wrote this as ==1, then acknowledged that all structs
    // are padded to a multiple of 2 bytes on this compiler, which
    // is sensible, so I've fixed the test.
    test_execute (__func__, sizeof (struct cbf) == 2);
}

TESTFUNC tests[] =
{
    t_bf_init,
    t_bf_setget,
    t_bf_rmw,
    t_bf_calls,
    t_bf_char
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main (void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
