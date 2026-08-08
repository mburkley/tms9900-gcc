#include "tap.h"
#include "params.h"

/* reconstruct an actual 32-bit result via 32 pass/fail bit probes */
static void probe_bits(const char *name, long got, long expect)
{
    int b;
    test_execute(name, got == expect);
    for (b = 0; b < 32; b++)
        test_execute("bit", ((got >> b) & 1) == ((expect >> b) & 1));
}

/* signed long / 5 across the 65535 boundary */
void t_div5_boundary(void)
{
    static const struct { long in; long out; } c[] = {
        {  65530, 13106 },
        {  65535, 13107 },
        {  65536, 13107 },
        {  70000, 14000 },
        { 100000, 20000 }
    };
    unsigned i;
    for (i = 0; i < sizeof(c)/sizeof(c[0]); i++) {
        set_sl_x(c[i].in);
        probe_bits("div5", sl_x / 5, c[i].out);
    }
}

/* unsigned long % n  -- exactly what rnd() computes */
void t_umod(void)
{
    static const struct { unsigned long a; unsigned long b; long out; } c[] = {
        { 0x0001A5E0UL, 3, 0 },
        { 0x12345678UL, 3, 0 },
        { 0xABCD1234UL, 4, 0 },
        { 0xFFFFFFFFUL, 5, 0 },
        { 0x80000000UL, 10, 8 },
        { 0x00012345UL, 9, 0 }
    };
    unsigned i;
    for (i = 0; i < sizeof(c)/sizeof(c[0]); i++) {
        set_ul_x(c[i].a); set_ul_y(c[i].b);
        probe_bits("umod", (long)(ul_x % ul_y), c[i].out);
    }
}

/* unsigned long / n with high word set */
void t_udiv(void)
{
    static const struct { unsigned long a; unsigned long b; long out; } c[] = {
        { 0x00010000UL, 5, 13107 },
        { 0x000186A0UL, 5, 20000 },
        { 0xFFFFFFFFUL, 65536, 65535 }
    };
    unsigned i;
    for (i = 0; i < sizeof(c)/sizeof(c[0]); i++) {
        set_ul_x(c[i].a); set_ul_y(c[i].b);
        probe_bits("udiv", (long)(ul_x / ul_y), c[i].out);
    }
}

#include "ltoa.c"

TESTFUNC tests[] =
{
    t_div5_boundary,
    t_umod,
    t_udiv
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);
    return 0;
}
