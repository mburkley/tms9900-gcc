#include "tap.h"
static const long in[] = { 0L, 4L, 5L, 65530L, 65535L, 65536L, 70000L, 100000L, 24000L, 108000L, 2000000000L, -7L, -2000L, -100000L, -65536L, -65537L };
static const long dq[] = { 0L, 0L, 1L, 13106L, 13107L, 13107L, 14000L, 20000L, 4800L, 21600L, 400000000L, -1L, -400L, -20000L, -13107L, -13107L };
static const long dr[] = { 0L, 4L, 0L, 0L, 0L, 1L, 0L, 0L, 0L, 0L, 0L, -2L, 0L, 0L, -1L, -2L };
static const unsigned long uq[] = { 0UL, 0UL, 1UL, 13106UL, 13107UL, 13107UL, 14000UL, 20000UL, 4800UL, 21600UL, 400000000UL, 858993457UL, 858993059UL, 858973459UL, 858980352UL, 858980351UL };
volatile long vx;
long __attribute__((noinline)) d5 (long x) { return x / 5; }
long __attribute__((noinline)) m5 (long x) { return x % 5; }
unsigned long __attribute__((noinline)) u5 (unsigned long x) { return x / 5; }
void t_d5 (void) { for (unsigned i = 0; i < 16; i++) { vx = in[i]; test_execute ("d5", d5 (vx) == dq[i]); } }
void t_m5 (void) { for (unsigned i = 0; i < 16; i++) { vx = in[i]; test_execute ("m5", m5 (vx) == dr[i]); } }
void t_u5 (void) { for (unsigned i = 0; i < 16; i++) { vx = in[i]; test_execute ("u5", u5 (vx) == uq[i]); } }
void t_inline (void) { for (unsigned i = 0; i < 16; i++) { vx = in[i]; long x = vx; test_execute ("inl", x / 5 == dq[i]); } }
TESTFUNC tests[] = { t_d5, t_m5, t_u5, t_inline };
#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))
int main (void) { test_run (tests, TEST_COUNT); return 0; }
