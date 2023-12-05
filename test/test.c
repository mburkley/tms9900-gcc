#ifdef __tms9900__
#include <vdp.h>
#else
#include <stdio.h>
#endif

#include "tap.h"

#if 0

// Disabled for now until float funcs are implemented in libgcc

static void test_double_init (void)
{
    double x = 1.2;
    x *= 3.7;

    test_execute (__func__, x>4.3&&x<4.5);
}
#endif

#if 1
static void test_mul_short_long (void)
{
    int x =6;
    int y =7;
    long z;
    z = x*y;

    test_execute (__func__, z==42);
}

static int stack_func_arg (int a, int b, int c, int d, ...)
{
    int w = a;
    int x = b;
    int y = c;
    int z = d;

    return (w==65 && x==66 && y==67 && z==68);
}

static void test_stack_func_arg (void)
{
    int a = 65;
    int b = 66;
    int c = 67;
    int d = 68;
    int e = 69;
    int f = 70;
    int g = 71;
    int h = 72;

    int res = stack_func_arg (a, b, c, d, e, f, g, h);
    test_execute (__func__, res != 0);

    res = stack_func_arg (a, b, c, d);
    test_execute (__func__, res != 0);

    res = stack_func_arg (a, b, c, d, e, f, g, h, 0, 0);
    test_execute (__func__, res != 0);
}
#endif

static void test_us_byte_divmod()
{
    unsigned char x = -7; // should be interpreted as 249
    unsigned char y = 5;
    unsigned char z;

    z= x / y;
    // printf("# div res=%d\n", (int)z);
    test_execute (__func__, z==49);

    z= x % y;
    // printf("# mod res=%d\n", (int)z);
    test_execute (__func__, z==4);
}

#if 1
static void test_s_byte_divmod()
{
    char x = 13;
    char y = -5;
    char z;

    z= x / y;
    // printf("# div res=%d\n", (int)z);
    test_execute (__func__, z==-2);

    z= x % y;
    // printf("# mod res=%d\n", (int)z);
    test_execute (__func__, z==3);
}

static void test_us_byte_mpy()
{
    unsigned char x = 7;
    unsigned char y = -9; //  should interpret as 247
    unsigned char z;

    z= x * y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==193);
}

static void test_s_byte_mpy()
{
    char x = 7;
    char y = -9;
    char z;

    z= x * y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==-63);
}
static void test_us_byte_add()
{
    unsigned char x = 7;
    unsigned char y = -9; //  should interpret as 247
    unsigned char z;

    z= x + y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==254);
}

static void test_s_byte_add()
{
    char x = 7;
    char y = -9;
    char z;

    z= x + y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==-2);
}
static void test_s_byte_extend_short()
{
    char x = -7;
    short y = x;

    // printf("# res=%d\n", (int)z);
    test_execute (__func__, y==-7);
}

static void test_us_byte_extend_short()
{
    unsigned char x = -7;
    unsigned short y = x;

    // printf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void test_s_byte_extend_long()
{
    char x = -7;
    long y = x;

    // printf("# res=%d\n", (int)y);
    test_execute (__func__, y==-7);
}

static void test_us_byte_extend_long()
{
    unsigned char x = -7;
    unsigned long y = x;

    // printf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void test_s_short_trunc_byte()
{
    short x = -7;
    char y = x;

    // printf("# res=%d\n", (int)z);
    test_execute (__func__, y==-7);
}

static void test_us_short_trunc_byte()
{
    unsigned short x = -7;
    unsigned char y = x;

    // printf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void test_s_long_trunc_byte()
{
    /*  For some bizarre reason, movhi expanded from movsi with a negative
     *  initialiser causes a compiler abort???? */
    // fails: long x = -7;
    // fails long x = 65529;
    // fails: long x = 32768;
    long x = 32767;
    char y = x;

    // printf("# res=%d\n", (int)y);
    test_execute (__func__, y==-1);
}

static void test_us_long_trunc_byte()
{
    // unsigned long x = -7;
    // unsigned long x = 65529;
    unsigned long x = 32767;
    unsigned char y = x;

    // printf("# res=%d\n", (int)y);
    test_execute (__func__, y==255);
}
#endif

typedef void (*FUNCPTR) (void);

FUNCPTR tests[] = 
{
    test_mul_short_long,
    test_stack_func_arg,
    test_us_byte_divmod,
    test_s_byte_divmod,
    test_us_byte_mpy,
    test_s_byte_mpy,
    test_us_byte_add,
    test_s_byte_add,
    test_s_byte_extend_short,
    test_us_byte_extend_short,
    test_s_byte_extend_long,
    test_us_byte_extend_long,
    test_s_short_trunc_byte,
    test_us_short_trunc_byte,
    test_s_long_trunc_byte,
    test_us_long_trunc_byte
};

#define TEST_COUNT (sizeof (tests) / sizeof (FUNCPTR))

int main(void)
{
#ifdef __tms9900__
    set_graphics(0);
    charset();
#endif

    printf ("1..%d\n", TEST_COUNT);

#if 0
    void test_double_init (void);
#endif

    int i;
    for (i = 0; i < TEST_COUNT; i++)
        tests[i]();
        
    test_report ();

#ifdef __tms9900__
    while(1);
#endif
    return 0;
}
