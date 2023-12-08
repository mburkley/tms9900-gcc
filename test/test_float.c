#include "tap.h"

/*  Link to the simple ftoa method in libgcc. */
extern void tireal_ftoa(char*,char*);

static void t_double_add (void)
{
  double y=42.0;
  double x=33.3;
  double z= x+y;
  char s[30] = "****";
  tireal_ftoa ((char*)&z,s);
  printf("# 42+33.3==%s\n", s);
  test_execute (__func__, z==75.3);
}

static void t_double_sub (void)
{
  double y=42.0;
  double x=33.3;
  double z= x-y;
  char s[30] = "****";
  tireal_ftoa ((char*)&z,s);
  printf("# 42-33.3=%s\n", s);
  test_execute (__func__, z==-8.7);
}

static void t_double_mul (void)
{
  double y=3.0;
  double x=2.5;
  double z= x*y;
  char s[30] = "****";
  tireal_ftoa ((char*)&z,s);
  printf("# 3*2.5=%s\n", s);
  test_execute (__func__, z==7.5);
}

static void t_double_div (void)
{
  double y=15.0;
  double x=6.0;
  double z=x/y;
  char s[30] = "****";
  tireal_ftoa ((char*)&z,s);
  printf("# 6/15=%s\n", s);
  test_execute (__func__, z==0.4);
}

static void t_double_less (void)
{
  double y=15.0;
  double x=6.0;
  int z=(x<y);
  printf("# 6<15=%d\n", z);
  // z=(x>y);
  // printf("# gt=%d\n", z);
  // z=(x==y);
  // printf("# eq=%d\n", z);
  test_execute (__func__, x<y);
}

static void t_double_less_equal (void)
{
  double y=15.0;
  double x=6.0;
  int z=(x<=y);
  printf("# 6<=15=%d\n", z);
  test_execute (__func__, x<=y);
}

static void t_double_greater (void)
{
  double y=15.0;
  double x=6.0;
  int z=(y>x);
  printf("# 15>6=%d\n", z);
  test_execute (__func__, y>x);
}

static void t_double_greater_equal (void)
{
  double y=15.0;
  double x=6.0;
  int z=(y>=x);
  printf("# 15>=6=%d\n", z);
  test_execute (__func__, y>=x);
}

TESTFUNC tests[] = 
{
    t_double_add,
    t_double_sub,
    t_double_mul,
    t_double_div,
    t_double_less,
    t_double_less_equal,
    t_double_greater,
    t_double_greater_equal
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main (void)
{
    test_run (tests, TEST_COUNT);
}

