#include "tap.h"
#include "params.h"

/* 16-bit signed div/mod with negative divisors -- the inline divmodhi4 path.
 * ss_x/ss_y are 'short' globals (set_ss_*) so no constant folding. */
void t_div16(void)
{
    static const struct { short a; short b; short q; short r; } c[] = {
        {  13,  5,  2,  3 }, {  13, -5, -2,  3 }, { -13,  5, -2, -3 }, { -13, -5,  2, -3 },
        {  17,  5,  3,  2 }, {  17, -5, -3,  2 }, { -17,  5, -3, -2 }, { -17, -5,  3, -2 },
        { 30000,  7, 30000/7, 30000%7 }, { -30000,  7, -(30000/7), -(30000%7) },
        { 30000, -7, -(30000/7), 30000%7 }, { -30000, -7, 30000/7, -(30000%7) }
    };
    unsigned i;
    for (i = 0; i < sizeof(c)/sizeof(c[0]); i++) {
        set_ss_x(c[i].a); set_ss_y(c[i].b);
        test_execute("div16", (ss_x / ss_y) == c[i].q);
        test_execute("mod16", (ss_x % ss_y) == c[i].r);
        test_execute("id16",  (ss_x / ss_y) * ss_y + (ss_x % ss_y) == c[i].a);
    }
}

/* 8-bit signed mod with negative divisor -- divmodqi4 path */
void t_mod8(void)
{
    static const struct { char a; char b; char r; } c[] = {
        { 13, 5, 3 }, { 13, -5, 3 }, { -13, 5, -3 }, { -13, -5, -3 }
    };
    unsigned i;
    for (i = 0; i < 4; i++) {
        set_sc_x(c[i].a); set_sc_y(c[i].b);
        test_execute("mod8", (char)(sc_x % sc_y) == c[i].r);
    }
}

TESTFUNC tests[] = { t_div16, t_mod8 };
#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))
int main(void) { test_run (tests, TEST_COUNT); return 0; }
