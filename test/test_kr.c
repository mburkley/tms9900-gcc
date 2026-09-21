#include "tap.h"

int __attribute__ ((noinline)) fn1 (char c) { if (c) return 1; else return 0; }
int __attribute__ ((noinline)) fn2 (c) char c; { if (c) return 1; else return 0; }
char __attribute__ ((noinline)) fn3 (c) char c; { return c; }
int __attribute__ ((noinline)) fn4 (c) char c; { return c + 1; }

volatile char v;
volatile int r;

static void t_kr (void)
{
  static const char vals[] = {0, 1, -1, 0x7f, (char)0x80, 0x55};
  for (unsigned i = 0; i < sizeof vals; i++)
  {
    v = vals[i];
    int e = vals[i] != 0;
    r = fn1 (v); test_execute ("proto nonzero", r == e);
    r = fn2 (v); test_execute ("K&R nonzero", r == e);
    v = vals[i];
    test_execute ("K&R return", fn3 (v) == vals[i]);
    test_execute ("K&R c+1", fn4 (v) == vals[i] + 1);
  }
}

TESTFUNC tests[] = { t_kr };
#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))
int main (void) { test_run (tests, TEST_COUNT); }
