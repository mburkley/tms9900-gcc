#include "tap.h"

#define IFACTOR1 479 // random number to give 16 bit mult product
#define IFACTOR2 57
#define IMULRES (IFACTOR1*IFACTOR2)

#define LFACTOR1 25973 // mult res fits in 32 bits
#define LFACTOR2 47315
#define LMULRES (LFACTOR1*LFACTOR2)

#define BIN1 0x56784321;
#define BIN2 0x3a913c48;
#define BINOR 0x7ef97f69
#define BINAND 0x12100000

#define L_INP 0x12341234

#define LLSH_SLA3 0x91a091a0
#define LLSH_SLA4 0x23412340
#define LLSH_SLA16 0x12340000
#define LLSH_SLA17 0x24680000

#define R_INP 0x87341234
#define ARSH_SRA16 0xffff8734
#define ARSH_SRA17 0xffffc39a
#define ARSH_SRA4 0xf8734123
#define ARSH_SRA3 0xf0e68246

#define LRSH_SRL3 0x10e68246
#define LRSH_SRL4 0x08734123
#define LRSH_SRL16 0x00008734
#define LRSH_SRL17 0x0000439a

int ix = IFACTOR1;
int iy = IFACTOR2;
int32_t x = LFACTOR1;
int32_t y = LFACTOR2;
int32_t z;
int32_t dx =200000;
int32_t dy =66528;
int32_t iorx = BIN1;
int32_t iory = BIN2;
uint32_t uorx = BIN1;
uint32_t uory = BIN2;
int32_t linp = L_INP;
uint32_t rinp = R_INP;
int shc17 = 17; // shift count
int shc16 = 16; // shift count
int shc4 = 4; // shift count
int shc0 = 0; // shift count

#if 0
/*  Functions to setup test values.  Must not be inlined or else optimiser will
 *  just insert the result of the operation instead of actually doing it. */
void __attribute__ ((noinline)) set_x_y()
{
    ix =6;
    iy =7;
    x = 6;
    y = 7;
}
#endif

void t_mul_short_int32 (void)
{
    // set_ix_iy();
    z = ix*iy;

    test_execute (__func__, z==IMULRES);
}

void t_int32_mul (void)
{
    z = x*y;

    test_execute (__func__, z==LMULRES);
}

#include "ltoa.c"

void t_int32_div (void)
{
    int32_t z;
    z = dx/dy;

    dprintf("# z=%s\n", ltoa (z));
    test_execute (__func__, z==3);
}

void t_int32_or()
{
    int32_t z = iorx | iory;
    int32_t a = iorx | 0;
    int32_t b = iorx | -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==BINOR && a==iorx && b==-1);
}

void t_uint32_or()
{
    uint32_t z = uorx | uory;
    uint32_t a = uorx | 0;
    uint32_t b = uorx | 0xffffffff;

    test_execute (__func__, z==BINOR && a==uorx && b==0xffffffff);
}

void t_int32_and()
{
    int32_t z = iorx & iory;
    int32_t a = iorx & 0;
    int32_t b = iorx & -1;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==BINAND && a==0 && b==iorx);
}

void t_uint32_and()
{
    uint32_t z = uorx & uory;
    uint32_t a = uorx & 0;
    uint32_t b = uorx & 0xffffffff;

    dprintf("# z=%x a=%x b=%x\n", z, a, b);
    test_execute (__func__, z==BINAND && a==0 && b==uorx);
}

void t_int32_sla_lt16()
{
    int32_t x = linp;
    int32_t y;

    y = x << 3;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== LLSH_SLA3);
}

void t_int32_sla_var()
{
    int32_t x = linp;
    int32_t y;

    y = x<<shc4;
    dprintf("# y=%lx\n",y);
    test_execute (__func__, y== LLSH_SLA4);
}

void t_int32_sla_var_zero()
{
    int32_t x = linp;
    int32_t y;

    y = x<<shc0;
    dprintf("# y=%lx\n",y);
    test_execute (__func__, y== L_INP);
}

void t_int32_sla_gt16()
{
    int32_t x = linp;
    int32_t y;

    y=x<<shc17;
    dprintf("# y=%lx\n",y);
    test_execute (__func__, y== LLSH_SLA17);
}

void t_int32_sla_16()
{
    int32_t x = linp;
    int32_t y;

    // Use a var or optimiser will reduce
    y=x<<shc16;
    dprintf("# y=%d\n",y);
    test_execute (__func__, y== LLSH_SLA16);
}

void t_int32_srl_lt16()
{
    uint32_t x = rinp;
    uint32_t y;

    y = x >> 3;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== LRSH_SRL3);
}

void t_int32_srl_var()
{
    uint32_t x = rinp;
    uint32_t y;

    y = x>>shc4;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== LRSH_SRL4);
}

void t_int32_srl_gt16()
{
    uint32_t x = rinp;
    uint32_t y;

    y=x>>shc17;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== LRSH_SRL17);
}

void t_int32_srl_16()
{
    uint32_t x = rinp;
    uint32_t y;

    y=x>>shc16;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== LRSH_SRL16);
}


void t_int32_sra_lt16()
{
    int32_t x = rinp;
    int32_t y;

    y = x >> 3;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== ARSH_SRA3);
}

void t_int32_sra_var()
{
    int32_t x = rinp;
    int32_t y;

    y = x>>shc4;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== ARSH_SRA4);
}

void t_int32_sra_gt16()
{
    int32_t x = rinp;
    int32_t y;

    y=x>>shc17;
    dprintf("# y=%x\n", y);
    test_execute (__func__, y== ARSH_SRA17);
}

void t_int32_sra_16()
{
    int32_t x = rinp;
    int32_t y;

    y=x>>shc16;
    dprintf("# y=%x\n",y);
    test_execute (__func__, y== ARSH_SRA16);
}

// TODO int32_div is a known failing test case so has been temporarily removed

TESTFUNC tests[] = 
{
    t_mul_short_int32,
    t_int32_mul,
#if 0
    t_int32_div,
#endif
    t_int32_or,
    t_uint32_or,
    t_int32_and,
    t_uint32_and,
    t_int32_sla_lt16,
    t_int32_sla_var,
    t_int32_sla_var_zero,
    t_int32_sla_gt16,
    t_int32_sla_16,
    t_int32_srl_lt16,
    t_int32_srl_var,
    t_int32_srl_gt16,
    t_int32_srl_16,
    t_int32_sra_lt16,
    t_int32_sra_var,
    t_int32_sra_gt16,
    t_int32_sra_16
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
