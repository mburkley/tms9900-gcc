#include "tap.h"
#include "params.h"

static void t_uc_divmod()
{
    set_uc_x (-7); // should be interpreted as 249
    set_uc_y (5);

    uc_z = uc_x / uc_y;

    dprintf("# div res=%d\n", (int)uc_z);
    test_execute (__func__, uc_z==49);

    uc_z = uc_x % uc_y;

    dprintf("# mod res=%d\n", (int)uc_z);
    test_execute (__func__, uc_z==4);
}

static void t_sc_divmod()
{
    set_uc_x (13);
    set_uc_y (-5);

    sc_z = sc_x / sc_y;

    dprintf("# div res=%d\n", (int)sc_z);
    test_execute (__func__, sc_z==-2);

    sc_z = sc_x % sc_y;

    dprintf("# mod res=%d\n", (int)sc_z);
    test_execute (__func__, sc_z==3);
}

static void t_uc_mpy()
{
    set_uc_x (7);
    set_uc_y (-9); //  should interpret as 247

    uc_z = uc_x * uc_y;

    dprintf("# res=%d\n", (int)uc_z);
    test_execute (__func__, uc_z==193);
}

static void t_sc_mpy()
{
    set_sc_x (7);
    set_sc_y (-9);

    sc_z = sc_x * sc_y;

    dprintf("# res=%d\n", (int)sc_z);
    test_execute (__func__, sc_z==-63);
}

static void t_sc_extend_ss()
{
    set_sc_x (-7);

    ss_y = sc_x;

    dprintf("# res=%d\n", (int)ss_y);
    test_execute (__func__, ss_y==-7);
}

static void t_uc_extend_us()
{
    set_uc_x (-7);

    us_y = uc_x;

    dprintf("# res=%d\n", (int)us_y);
    test_execute (__func__, us_y==249);
}

static void t_sc_extend_sl()
{
    set_sc_x (-7);

    sl_y = sc_x;

    dprintf("# res=%d\n", (int)sl_y);
    test_execute (__func__, sl_y==-7);
}

static void t_uc_extend_ul()
{
    set_uc_x (-7);

    ul_y = uc_x;

    dprintf("# res=%d\n", (int)ul_y);
    test_execute (__func__, ul_y==249);
}

static void t_ss_trunc_sc()
{
    set_ss_x (-7);

    sc_y = ss_x;

    dprintf("# res=%d\n", (int)sc_y);
    test_execute (__func__, sc_y==-7);
}

static void t_us_trunc_uc()
{
    set_us_x (-7);

    uc_y = us_x;

    dprintf("# res=%d\n", (int)uc_y);
    test_execute (__func__, uc_y==249);
}

static void t_sl_trunc_sc()
{
    set_sl_x (-7);

    sc_y = sl_x;

    dprintf("# res=%d\n", (int)sc_y);
    test_execute (__func__, sc_y==-7);
}

static void t_ul_trunc_uc()
{
    unsigned long x = -7;
    // set_ul_x (-7);
    set_ul_x (x);
    // unsigned long x = 65529;
    // unsigned long x = 32767;

    uc_y = ul_x;

    dprintf("# res=%d\n", (int)uc_y);
    test_execute (__func__, uc_y==249);
}

void test_ss_add()
{
    set_ss_x (-42);
    set_ss_y (32743);

    ss_z = ss_x + ss_y;

    // printf("# res=%d\n", (int)ss_z);
    test_execute (__func__, ss_z==32701);
}

static void test_us_add()
{
    set_us_x (32760);
    set_us_y (42);

    us_z = us_x + us_y;

    printf("# res=%d\n", (int)us_z);
    test_execute (__func__, us_z==32802);
}

static void t_sc_or()
{
    set_sc_x (0x32);
    set_sc_y (0x48);

    char z = sc_x | sc_y;
    char a = sc_x | 0;
    char b = sc_x | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7a && a==sc_x && b==-1);
}

static void t_uc_or()
{
    set_uc_x (0x32);
    set_uc_y (0x48);

    unsigned char z = uc_x | uc_y;
    unsigned char a = uc_x | 0;
    unsigned char b = uc_x | 0xff;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7a && a==uc_x && b==0xff);
}

static void t_ss_or()
{
    set_ss_x (0x4321);
    set_ss_y (0x3c48);

    short z = ss_x | ss_y;
    short a = ss_x | 0;
    short b = ss_x | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7f69 && a==ss_x && b==-1);
}

static void t_us_or()
{
    set_us_x (0x4321);
    set_us_y (0x3c48);

    unsigned short z = us_x | us_y;
    unsigned short a = us_x | 0;
    unsigned short b = us_x | 0xffff;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x7f69 && a==us_x && b==0xffff);
}
static void t_sc_and()
{
    set_sc_x (0x57);
    set_sc_y (0x46);

    char z = sc_x & sc_y;
    char a = sc_x & 0;
    char b = sc_x & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x46 && a==0 && b==sc_x);
}

static void t_uc_and()
{
    set_uc_x (0x57);
    set_uc_y (0x46);

    unsigned char z = uc_x & uc_y;
    unsigned char a = uc_x & 0;
    unsigned char b = uc_x & 0xff;

    test_execute (__func__, z==0x46 && a==0 && b==uc_x);
}

static void t_ss_and()
{
    set_ss_x (0x5735);
    set_ss_y (0x4669);

    short z = ss_x & ss_y;
    short a = ss_x & 0;
    short b = ss_x & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==0x4621 && a==0 && b==ss_x);
}

static void t_us_and()
{
    set_us_x (0x5735);
    set_us_y (0x4669);

    unsigned short z = us_x & us_y;
    unsigned short a = us_x & 0;
    unsigned short b = us_x & 0xffff;

    test_execute (__func__, z==0x4621 && a==0 && b==us_x);
}

static void t_sc_sra ()
{
    set_sc_x (0x80);

    sc_x >>= 1;

    dprintf("# x=%X\n", sc_x);
    test_execute (__func__, (unsigned char)sc_x==0xc0);
}

static void t_sc_sra_var ()
{
    set_sc_x (0x80);
    set_sc_y (1);

    sc_x >>= sc_y;

    dprintf("# x=%X\n", sc_x);
    test_execute (__func__, (unsigned char)sc_x==0xc0);
}

static void t_uc_srl ()
{
    set_uc_x (0x80);

    uc_x >>= 1;

    dprintf("# x=%X\n", uc_x);
    test_execute (__func__, uc_x==0x40);
}

static void t_sc_sla ()
{
    set_sc_x (0x81);

    sc_x <<= 1;

    dprintf("# x=%X\n", sc_x);
    test_execute (__func__, sc_x==0x02);
}

static void t_sc_sla_var ()
{
    set_sc_x (0x81);
    set_sc_y (1);

    sc_x <<= sc_y;

    dprintf("# x=%X\n", sc_x);
    test_execute (__func__, sc_x==0x02);
}

static void t_uc_sla ()
{
    set_uc_x (0x81);

    uc_x <<= 1;

    dprintf("# x=%X\n", uc_x);
    test_execute (__func__, uc_x==0x02);
}

static void t_ss_sra ()
{
    set_ss_x (0x8040);

    ss_x >>= 1;

    dprintf("# x=%X\n", ss_x);
    test_execute (__func__, (unsigned short)ss_x==0xc020);
}

static void t_ss_sra_var ()
{
    set_ss_x (0x8040);
    set_ss_y (1);

    ss_x >>= ss_y;

    dprintf("# x=%X\n", ss_x);
    test_execute (__func__, (unsigned short)ss_x==0xc020);
}

static void t_us_srl ()
{
    set_us_x (0x8040);

    us_x >>= 1;

    dprintf("# x=%X\n", us_x);
    test_execute (__func__, us_x==0x4020);
}

static void t_ss_sla ()
{
    set_ss_x (0x8124);

    ss_x <<= 1;

    dprintf("# x=%X\n", ss_x);
    test_execute (__func__, ss_x==0x0248);
}

static void t_ss_sla_var ()
{
    set_ss_x (0x8124);
    set_ss_y (1);

    ss_x <<= ss_y;

    dprintf("# x=%X\n", ss_x);
    test_execute (__func__, ss_x==0x0248);
}

static void t_us_sla ()
{
    set_us_x (0x8124);

    us_x <<= 1;

    dprintf("# x=%X\n", us_x);
    test_execute (__func__, us_x==0x0248);
}

static void t_sc_add()
{
    set_sc_x (7);
    set_sc_y (-9);

    sc_z = sc_x + sc_y;

    dprintf("# res=%d\n", (int)sc_z);
    test_execute (__func__, sc_z==-2);
}

static void t_uc_add()
{
    set_uc_x (7);
    set_uc_y (-9); //  should interpret as 247

    uc_z = uc_x + uc_y;

    dprintf("# res=%d\n", (int)uc_z);
    test_execute (__func__, uc_z==254);
}

static void t_ss_add ()
{
    set_ss_x (0x4934);
    set_ss_y (0x2723);

    ss_y += ss_x;

    dprintf("# y=%X\n", ss_y);
    test_execute (__func__, ss_y==0x7057);
}

static void t_us_add ()
{
    set_us_x (0x4934);
    set_us_y (0x2723);

    us_y += us_x;

    dprintf("# y=%X\n", us_y);
    test_execute (__func__, us_y==0x7057);
}

static void t_sc_sub ()
{
    set_sc_x (0x49);
    set_sc_y (0x27);

    sc_y -= sc_x;

    dprintf("y=%x\n", sc_y);
    test_execute (__func__, (unsigned char) sc_y==0xde);
}

static void t_uc_sub ()
{
    set_uc_x (0x49);
    set_uc_y (0x27);

    uc_y -= uc_x;

    dprintf("y=%x\n", uc_y);
    test_execute (__func__, uc_y==0xde);
}

static void t_sc_imm_sub (void)
{
    set_sc_x (0x49);

    sc_y = sc_x - 0x27;

    dprintf("y=%x\n", sc_y);
    test_execute (__func__, sc_y==0x22);
}

static void t_sc_sub_imm (void)
{
    set_sc_x (0x27);

    sc_y = 0x49 - sc_x;

    dprintf("y=%x\n", sc_y);
    test_execute (__func__, sc_y==0x22);
}

static void t_ss_sub ()
{
    set_ss_x (0x4934);
    set_ss_y (0x2723);

    ss_y -= ss_x;

    dprintf("y=%x\n", ss_y);
    test_execute (__func__, (unsigned short)ss_y==0xddef);
}

static void t_us_sub ()
{
    set_us_x (0x4934);
    set_us_y (0x2723);

    us_y -= us_x;

    dprintf("y=%x\n", us_y);
    test_execute (__func__, us_y==0xddef);
}

void t_cmp_sc()
{
    set_sc_x (3);
    set_sc_y (4);

    char lt=(sc_x<sc_y);
    char gt=(sc_x>sc_y);
    char eq=(sc_x==sc_y);

    test_execute (__func__, lt&&!eq&&!gt);
}

void t_cmp_ss()
{
    set_ss_x (3);
    set_ss_y (4);

    int lt=(ss_x<ss_y);
    int gt=(ss_x>ss_y);
    int eq=(ss_x==ss_y);

    test_execute (__func__, lt&&!eq&&!gt);
}

TESTFUNC tests[] = 
{
    t_uc_divmod,
    t_sc_divmod,
    t_uc_mpy,
    t_sc_mpy,
    t_sc_extend_ss,
    t_uc_extend_us,
    t_sc_extend_sl,
    t_uc_extend_ul,
    t_ss_trunc_sc,
    t_us_trunc_uc,
    t_sl_trunc_sc,
    t_ul_trunc_uc,
    t_sc_or,
    t_uc_or,
    t_ss_or,
    t_us_or,
    t_sc_and,
    t_uc_and,
    t_ss_and,
    t_us_and,
    t_sc_sra,
    t_sc_sra_var,
    t_uc_srl,
    t_sc_sla,
    t_sc_sla_var,
    t_uc_sla,
    t_ss_sra,
    t_ss_sra_var,
    t_us_srl,
    t_ss_sla,
    t_ss_sla_var,
    t_us_sla,
    t_sc_add,
    t_uc_add,
    t_ss_add,
    t_us_add,
    t_sc_sub,
    t_uc_sub,
    t_sc_imm_sub,
    t_sc_sub_imm,
    t_ss_sub,
    t_us_sub,
    t_cmp_sc,
    t_cmp_ss
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
