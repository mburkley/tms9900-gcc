#include "tap.h"
#include "params.h"

/* C99 6.5.5: remainder takes the sign of the DIVIDEND (a in a%b).
 * (a/b)*b + a%b == a. */
void t_mod(void)
{
    static const struct { long a; long b; long r; } c[] = {
        {  13,  5,  3 }, {  13, -5,  3 }, { -13,  5, -3 }, { -13, -5, -3 },
        {  17,  5,  2 }, {  17, -5,  2 }, { -17,  5, -2 }, { -17, -5, -2 },
        { 100000,  7,  100000L % 7 },  { -100000,  7, -(100000L % 7) },
        { 100000, -7,  100000L % 7 },  { -100000, -7, -(100000L % 7) }
    };
    unsigned i;
    for (i = 0; i < sizeof(c)/sizeof(c[0]); i++) {
        set_sl_x(c[i].a); set_sl_y(c[i].b);
        long got = sl_x % sl_y;
        test_execute("mod", got == c[i].r);
        /* identity check */
        test_execute("identity", (sl_x / sl_y) * sl_y + got == c[i].a);
    }
}

TESTFUNC tests[] = { t_mod };
#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))
int main(void) { test_run (tests, TEST_COUNT); return 0; }
