#include "tap.h"

static void t_us_char_divmod()
{
    unsigned char x = -7; // should be interpreted as 249
    unsigned char y = 5;
    unsigned char z;

    z= x / y;
    dprintf("# div res=%d\n", (int)z);
    test_execute (__func__, z==49);

    z= x % y;
    dprintf("# mod res=%d\n", (int)z);
    test_execute (__func__, z==4);
}

#if 1
static void t_s_char_divmod()
{
    char x = 13;
    char y = -5;
    char z;

    z= x / y;
    dprintf("# div res=%d\n", (int)z);
    test_execute (__func__, z==-2);

    z= x % y;
    dprintf("# mod res=%d\n", (int)z);
    test_execute (__func__, z==3);
}

static void t_us_char_mpy()
{
    unsigned char x = 7;
    unsigned char y = -9; //  should interpret as 247
    unsigned char z;

    z= x * y;
    dprintf("# res=%d\n", (int)z);
    test_execute (__func__, z==193);
}

static void t_s_char_mpy()
{
    char x = 7;
    char y = -9;
    char z;

    z= x * y;
    dprintf("# res=%d\n", (int)z);
    test_execute (__func__, z==-63);
}

static void t_s_char_ext_short()
{
    char x = -7;
    short y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==-7);
}

static void t_us_char_ext_short()
{
    unsigned char x = -7;
    unsigned short y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void t_s_char_ext_long()
{
    char x = -7;
    long y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==-7);
}

static void t_us_char_ext_long()
{
    unsigned char x = -7;
    unsigned long y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void t_s_short_tr_char()
{
    short x = -7;
    char y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==-7);
}

static void t_us_short_tr_char()
{
    unsigned short x = -7;
    unsigned char y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void t_s_long_tr_char()
{
    long x = -7;
    char y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==-7);
}

static void t_us_long_tr_char()
{
    unsigned long x = -7;
    // unsigned long x = 65529;
    // unsigned long x = 32767;
    unsigned char y = x;

    dprintf("# res=%d\n", (int)y);
    test_execute (__func__, y==249);
}

static void test_s_short_add()
{
    short x = -42;
    short y = 32743;
    short z;

    z = x + y;
    // printf("# res=%d\n", (int)z);
    test_execute (__func__, z==32701);
}

static void test_us_short_add()
{
    unsigned short x = 32760;
    unsigned short y = 42;
    unsigned short z;

    z = x + y;
    printf("# res=%d\n", (int)z);
    test_execute (__func__, z==32802);
}

#endif

static void t_s_char_or()
{
    char x = 0x32;
    char y = 0x48;
    char z = x | y;
    char a = x | 0;
    char b = x | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7a && a==x && b==-1);
}

static void t_us_char_or()
{
    unsigned char x = 0x32;
    unsigned char y = 0x48;
    unsigned char z = x | y;
    unsigned char a = x | 0;
    unsigned char b = x | 0xff;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7a && a==x && b==0xff);
}

static void t_s_short_or()
{
    short x = 0x4321;
    short y = 0x3c48;
    short z = x | y;
    short a = x | 0;
    short b = x | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7f69 && a==x && b==-1);
}

static void t_us_short_or()
{
    unsigned short x = 0x4321;
    unsigned short y = 0x3c48;
    unsigned short z = x | y;
    unsigned short a = x | 0;
    unsigned short b = x | 0xffff;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7f69 && a==x && b==0xffff);
}
static void t_s_char_and()
{
    char x = 0x57;
    char y = 0x46;
    char z = x & y;
    char a = x & 0;
    char b = x & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x46 && a==0 && b==x);
}

static void t_us_char_and()
{
    unsigned char x = 0x57;
    unsigned char y = 0x46;
    unsigned char z = x & y;
    unsigned char a = x & 0;
    unsigned char b = x & 0xff;

    test_execute (__func__, z==0x46 && a==0 && b==x);
}

static void t_s_short_and()
{
    short x = 0x5735;
    short y = 0x4669;
    short z = x & y;
    short a = x & 0;
    short b = x & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x4621 && a==0 && b==x);
}

static void t_us_short_and()
{
    unsigned short x = 0x5735;
    unsigned short y = 0x4669;
    unsigned short z = x & y;
    unsigned short a = x & 0;
    unsigned short b = x & 0xffff;

    test_execute (__func__, z==0x4621 && a==0 && b==x);
}

static void t_s_char_sra ()
{
    char x = 0x80;
    x >>= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, (unsigned char)x==0xc0);
    x = 0x40;
    x>>=1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x20);
}

static void t_us_char_srl ()
{
    unsigned char x = 0x80;
    x >>= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x40);
}

static void t_s_char_sla ()
{
    char x = 0x81;
    x <<= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x02);
}

static void t_us_char_sla ()
{
    unsigned char x = 0x81;
    x <<= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x02);
}

static void t_s_short_sra ()
{
    // temporarily disabled to fit into cart
    #if 1
    short x = 0x8040;
    x >>= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, (unsigned short)x==0xc020);
    x = 0x4020;
    x>>=1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x2010);
    #endif
}

static void t_us_short_srl ()
{
    unsigned short x = 0x8040;
    x >>= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x4020);
}

static void t_s_short_sla ()
{
    short x = 0x8124;
    x <<= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x0248);
}

static void t_us_short_sla ()
{
    unsigned short x = 0x8124;
    x <<= 1;
    dprintf("# x=%X\n", x);
    test_execute (__func__, x==0x0248);
}

static void t_s_char_add()
{
    char x = 7;
    char y = -9;
    char z;

    z= x + y;
    dprintf("# res=%d\n", (int)z);
    test_execute (__func__, z==-2);
}
static void t_us_char_add()
{
    unsigned char x = 7;
    unsigned char y = -9; //  should interpret as 247
    unsigned char z;

    z= x + y;
    dprintf("# res=%d\n", (int)z);
    test_execute (__func__, z==254);
}

static void t_s_short_add ()
{
    short x = 0x4934;
    short y = 0x2723;
    y += x;
    dprintf("# y=%X\n", y);
    test_execute (__func__, y==0x7057);
}

static void t_us_short_add ()
{
    // temporarily disabled to fit into cart
    #if 1
    unsigned short x = 0x4934;
    unsigned short y = 0x2723;
    y += x;
    dprintf("# y=%X\n", y);
    test_execute (__func__, y==0x7057);
    #endif
}

static void t_s_char_sub ()
{
    char x = 0x49;
    char y = 0x27;
    y -= x;
    dprintf("y=%x\n", y);
    test_execute (__func__, (unsigned char) y==0xde);
}

static void t_us_char_sub ()
{
    // temporarily disabled to fit into cart
    #if 1
    unsigned char x = 0x49;
    unsigned char y = 0x27;
    y -= x;
    dprintf("y=%x\n", y);
    test_execute (__func__, y==0xde);
    #endif
}

static void t_s_short_sub ()
{
    short x = 0x4934;
    short y = 0x2723;
    y -= x;
    dprintf("y=%x\n", y);
    test_execute (__func__, (unsigned short)y==0xddef);
}

static void t_us_short_sub ()
{
    unsigned short x = 0x4934;
    unsigned short y = 0x2723;
    y -= x;
    dprintf("y=%x\n", y);
    test_execute (__func__, y==0xddef);
}

void t_cmp_s_char()
{
    char x=3;
    char y=4;
    char lt=(x<y);
    char gt=(x>y);
    char eq=(x==y);
    test_execute (__func__, lt&&!eq&&!gt);
}

void t_cmp_s_short()
{
    int x=3;
    int y=4;
    int lt=(x<y);
    int gt=(x>y);
    int eq=(x==y);
    test_execute (__func__, lt&&!eq&&!gt);
}

TESTFUNC tests[] = 
{
    t_us_char_divmod,
    t_s_char_divmod,
    t_us_char_mpy,
    t_s_char_mpy,
    t_s_char_ext_short,
    t_us_char_ext_short,
    t_s_char_ext_long,
    t_us_char_ext_long,
    t_s_short_tr_char,
    t_us_short_tr_char,
    t_s_long_tr_char,
    t_us_long_tr_char,
    t_s_char_or,
    t_us_char_or,
    t_s_short_or,
    t_us_short_or,
    t_s_char_and,
    t_us_char_and,
    t_s_short_and,
    t_us_short_and,
    t_s_char_sra,
    t_us_char_srl,
    t_s_char_sla,
    t_us_char_sla,
    t_s_short_sra,
    t_us_short_srl,
    t_s_short_sla,
    t_us_short_sla,
    t_s_char_add,
    t_us_char_add,
    t_s_short_add,
    t_us_short_add,
    t_s_char_sub,
    t_us_char_sub,
    t_s_short_sub,
    t_us_short_sub,
    t_cmp_s_char,
    t_cmp_s_short
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

#if 0
    void test_double_init (void);
#endif

    return 0;
}
