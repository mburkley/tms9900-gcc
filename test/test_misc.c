/*
 *  This is a collection of miscellaneous tests, mostly added due to bug reports
 *  on the forums.  Comments ahead of each test should describe how the failure
 *  was observed.  Many of these tests only fail with -O2 enabled.
 *
 *  When creating tests that should run when optimised, avoid initialising vars
 *  inside the test function as the compiler will short circuit the test
 *  otherise.  Also don't declare funcs as static and if necessary declare as
 *  noinline.  Static vars may need to be declared volatile.
 */

#include "tap.h"
#include <stdarg.h>

void t_version (void)
{
    dprintf ("# gcc v=%s, major=%d minor=%d\n",
            __VERSION__,
            __TMS9900_PATCH_MAJOR__,
            __TMS9900_PATCH_MINOR__);

    test_execute (__func__, __TMS9900_PATCH_MAJOR__==1);
}

/*  Take a variable number of params, verify they are in ascending order, sum
 *  them together and verify the expected sum.  Use this function to ensure
 *  parameter ordering is correct even when the number of parameters is too many
 *  to be passed in registers and must be passed by stack.
 */
static void stack_func_arg (int count, int expect_sum, ...)
{
    int i;
    va_list ap;
    int sum = 0;
    int ascending = 1;
    int param;
    int last = 0;
    va_start(ap, expect_sum);

    for (i = 0; i < count; i++)
    {
        param = va_arg(ap, int);
        ascending = ascending && (param >= last);
        last = param;
        sum += param;
    }

    dprintf ("# sum=%d ascend=%d\n", sum, ascending);
    va_end(ap);

    test_execute (__func__, (sum == expect_sum) && ascending);
}

void t_stack_func_arg_small (void)
{
    stack_func_arg (4, 42, 1, 10, 15, 16);
}

void t_stack_func_arg_large (void)
{
    // Use Fibonacci, why not
    stack_func_arg (15, 986, 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377);
}

void t_for_break(void)
{
    int i;

    for (i = 0; i < 4; i++)
        if (i==2)
            break;

    dprintf("# i=%d\n", i);
    test_execute (__func__, i==2);
}

void test_array (const int k)
{
   int v[k];
   int i;
   int pass = 1;

   for (i = 0; i < k; i++)
   {
      v[i] = i;
   }

   dprintf ("# size int[5] == %d\n", (int) sizeof (v));
   pass = pass && (sizeof(v)==10);
   for (i = 0; i < k; i++)
   {
      dprintf ("# %d\n", v[i]);
      pass = pass && (v[i]==i);
   }
   test_execute (__func__, pass);
}

void t_var_byte_array()
{
   test_array (5);
}

void t_byte_array()
{
    int w;
    char x[1];
    int y;
    char z[3];
    int a;
    int pass=1;
    char s[6]="hello";

    x[0]=0x55;
    z[0]=0x11;
    z[1]=0x22;
    z[2]=0x33;
    w=42;
    y=84;
    a=21;

    dprintf("# z[] = { %X, %X, %X }\n",
            (int)z[0], (int)z[1], (int)z[2]);
    dprintf("# x[0]=%X w=%d y=%d a=%d\n",
           (int)x[0], w, y, a);

    pass = pass && (x[0]==0x55);
    pass = pass && (z[0]==0x11);
    pass = pass && (z[1]==0x22);
    pass = pass && (z[2]==0x33);
    pass = pass && (w==42);
    pass = pass && (y==84);
    pass = pass && (a==21);

    test_execute (__func__, pass);
}

void t_long_char_str ()
{
    /*  The limitation of 1024 chars in a single string is an assembler limit,
     *  not a compiler limit.  The size is hardcoded in
     *  gas/config/tc-tms9900:566.
     *
     *  Limiting the string to below 1024 for now as increasing the buffer size
     *  in gas didn't work.  Possibly need to tweak the frags or something as
     *  well.
     *
     *  There was also an issue in tms9900.c where it wasn't breaking up large
     *  text blocks.  Fixing this makes the issue with gas a non issue.
     *
     *  This test is disabled by default as wasting 2k of memory when trying to
     *  run in a cartridge limits the other testing we can do.
     */
#if 0
    const char s[] =
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
    "hello";
    int l = strlen(s);
    printf("# len=%d\n", l);
    test_execute (__func__, l==69);
#endif
}

void t_inline_clobber()
{
    /* Declare an inline asm that clobbers R11 */

    __asm__(
    "li r11,0x55aa\n"
    :
    :
    : "r11"
    );

    test_execute (__func__, 1);
}


// return the distance between a and b
unsigned char distance(unsigned char a, unsigned char b) {
	if (a>b) return a-b; else return b-a;
}

void t_distance (void)
{
    unsigned char x = distance (5, 10);
    test_execute (__func__, x==5);
}

volatile unsigned int peep_j;
volatile char peep_b;

// Compile with: -Os


void t_peephole (void)
{
    // ;; Optimization for (unsigned char)X < N
    if ((unsigned char)peep_b < 2)
        peep_j = 0;
    // ;; Optimization for (unsigned char)X >= N
    if ((unsigned char)peep_b >= 2)
        peep_j = 0;
    // ;; Optimization for (unsigned char)X <= N
    if ((unsigned char)peep_b <= 2)
        peep_j = 0;
    // ;; Optimization for (char)X > N
    if (peep_b > 2)
        peep_j = 0;
    // ;; Optimization for (char)X < N
    if (peep_b < 2)
        peep_j = 0;
    // ;; Optimization for (char)X >= N
    if (peep_b >= 2)
        peep_j = 0;
    // ;; Optimization for (char)X <= N
    if (peep_b <= 2)
        peep_j = 0;
}

int f2(int x)
{
    return 42;
}

void t_tbd_f3(void)
{
    dprintf("hi2");
}

/*  Test for bug where constants are expanded signed and then verified unsigned
 *  in combine.c:676 (do_SUBST)
 */
#if 1
#define VDP_MODE1_16K			(unsigned char)0x80		// set 16k mode (4k mode if cleared)
#define VDP_MODE1_UNBLANK		(unsigned char)0x40		// set to enable display, clear to blank it
#define VDP_MODE1_INT			(unsigned char)0x20		// enable VDP interrupts
#define VDP_MODE1_TEXT			(unsigned char)0x10		// set text mode
#endif

unsigned char test_set_text_raw() {
	unsigned char unblank = VDP_MODE1_16K | VDP_MODE1_UNBLANK | VDP_MODE1_TEXT | VDP_MODE1_INT;

	return unblank;
}

void t_subst (void)
{
    test_set_text_raw();
}

TESTFUNC tests[] = 
{
    t_version,
    t_stack_func_arg_small,
    t_stack_func_arg_large,
    t_long_char_str,
    t_for_break,
    t_var_byte_array,
    t_byte_array,
    t_inline_clobber,
    t_distance,
    t_subst
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
