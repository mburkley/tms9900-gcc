#include <vdp.h>

#include "tap.h"

#if 0
void test_double_init (void)
{
    double x = 1.2;
    x *= 3.7;

    test_execute (__func__, x>4.3&&x<4.5);
}
#endif

#if 1
void test_mul_short_long (void)
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

void test_stack_func_arg (void)
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

void test_us_byte_divmod()
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
void test_s_byte_divmod()
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

void test_us_byte_mpy()
{
    unsigned char x = 7;
    unsigned char y = -9; //  should interpret as 247
    unsigned char z;

    z= x * y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==193);
}

void test_s_byte_mpy()
{
    char x = 7;
    char y = -9;
    char z;

    z= x * y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==-63);
}
#endif

int main(void)
{
    set_graphics(0);
    charset();

    printf ("1..10\n");

    test_mul_short_long ();
    test_stack_func_arg ();
    test_us_byte_divmod();
    test_s_byte_divmod();
    test_us_byte_mpy();
    test_s_byte_mpy();

    test_report ();

    while(1);
    return 0;
}
