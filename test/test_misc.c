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
    printf ("# gcc v=%s, major=%d minor=%d\n",
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

unsigned char color;

/*  Replicating a bug in vdp_bmcolor where a swpb was missed in andhi3.  If
 *  compiled with -O2 the following output drops the extend of r2:

	andi r1, >F
	movb @color, r2
	andi r2, >F0
	soc  r2, r1
*/

void __attribute__ ((noinline)) setbackground(int c)
{
    color = (color & 0xF0) | (c & 0x0F);
}

void t_bitwise_replace()
{
    color = 0xaa;
    setbackground(0x55);
    printf("# color=%d\n", color);
    test_execute (__func__, color==0xa5);
}

int __attribute__ ((noinline)) test_mixed_params (unsigned char c, unsigned int len)
{
    return c+len;
}

/*  We are looking for a bug in movqi.  With -O2 in 1.27, this code produces:

	li   r2, >20
	movb r2, r1
	bl   @test_mixed_params

    and without -O2, the correct code:

	li   r2, >20
	li   r1, >2000
	bl   @test_mixed_params
*/
unsigned char test_return_mixed ()
{
    return test_mixed_params (32, 32);
}

void t_mixed_params()
{
    char x = test_return_mixed();
    test_execute (__func__, x==64);
}

/*  This test reproduces a faulty movqi issue reported by Tursi on Dec 24 */

char SpriteTab[10] = { 0x22, 0x44, 0x66 };
#define SHIP_C SpriteTab[1]
int playerXspeed = 0x3333;

void t_cmp_sub()
{
    #if 0
    SpriteTab[0] = 0x22;
    SpriteTab[1] = 0x44;
    SpriteTab[2] = 0x66;
    playerXspeed = 0x3333;
    #endif

    if (SHIP_C < 224-playerXspeed) SHIP_C+=playerXspeed;
    test_execute (__func__, SpriteTab[0] == 0x22 && SpriteTab[1] == 0x77 &&
    SpriteTab[2] == 0x66);
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
    t_bitwise_replace,
    t_mixed_params,
    t_cmp_sub
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
