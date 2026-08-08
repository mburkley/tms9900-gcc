#include "tap.h"
#include "params.h"

/* variable divisor -- deterministically failed at -Os before the fix */
void t_var(void)
{
    static const long in[] = { 2000, 24000, 108000, 100000, 700, 1500, -2000, 2000 };
    static const long dv[] = {    5,     2,      9,      5,   2,    3,     5,   -5 };
    static const long ex[] = {  400, 12000,  12000,  20000, 350,  500,  -400, -400 };
    unsigned i;
    for (i = 0; i < 8; i++) {
        set_sl_x(in[i]); set_sl_y(dv[i]);
        test_execute("var", (sl_x / sl_y) == ex[i]);
    }
}

/* constant /5 with a negative entry in the table -- failed before the fix */
void t_const(void)
{
    static const long in[] = { 2000, 24000, 108000, -2000, 140, 500 };
    static const long ex[] = {  400,  4800,  21600,  -400,  28, 100 };
    unsigned i;
    for (i = 0; i < 6; i++) {
        set_sl_x(in[i]);
        test_execute("const5", (sl_x / 5) == ex[i]);
    }
}

/* the exact Taipan shock: positive price /5 must stay positive */
void t_shock(void)
{
    static const long price[] = { 500, 700, 1500, 2100, 24000, 108000 };
    unsigned i;
    for (i = 0; i < 6; i++) {
        set_sl_x(price[i]);
        long cp = sl_x / 5;
        if (cp < 1) cp = 1;
        test_execute("shock_ge2", cp >= 2);   /* must NOT get clamped to 1 */
    }
}

TESTFUNC tests[] = { t_var, t_const, t_shock };
#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))
int main(void) { test_run (tests, TEST_COUNT); return 0; }
