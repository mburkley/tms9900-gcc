/*
 *  This is a collection of tests designed to catch issues with QI subregs of HI
 *  regs.
 */

#include "tap.h"
#include <stdarg.h>

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
    dprintf("# color=%d\n", color);
    test_execute (__func__, color==0xa5);
}

// int __attribute__ ((noinline)) test_mixed_params (unsigned char c, unsigned int len)
int __attribute__ ((noinline)) test_mixed_params (char c, unsigned int len)
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
int playerXspeed = 0x33;

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

#define uint8 unsigned char
#define NUM_SHOTS 9

int f18a;
int shc [NUM_SHOTS];
int ep [3];

#define spposn test_dummy_int
#define xx_VDP_SET_REGISTER test_dummy_void

void t_whoded() { 
    for (uint8 a=0; a<NUM_SHOTS; a++) {
        for (uint8 b=0; b<3; ++b) {
            spposn(b);
            if (shc[a] == 0) {
                if (f18a) {
                    xx_VDP_SET_REGISTER();
                } else {
                    xx_VDP_SET_REGISTER();
                }
                ep[b]-=1;
                break;
            }
        }
    }
    test_execute (__func__, 1==1);
}

void tttt(void * t) {
    int (*t2) (unsigned char c, unsigned int len) = t;
    t2 (32,32);
}

int fptr_result;

int ssss(unsigned char c, unsigned int len)
{
    fptr_result = len+(int)c;
}

void t_fptr_mixed (void)
{
    tttt(ssss);
    test_execute (__func__, fptr_result == 64);
}

unsigned char gSaveIntCnt2;
int haunted;
int hauntedIntArray[2];
char hauntedCharArray[4];

/*  Test for bug in addhi3 where subreg offset is passed */

void test_haunted (void) {
    int toggleval = (haunted + gSaveIntCnt2) & 0x001f;
    if (toggleval == 0x10)
    {
        haunted = 0;
    }
    else if (toggleval == 0)
    {
        haunted = 1;
    }

    test_execute (__func__, haunted == 0);
}

void t_add_subreg (void)
{
    haunted = 0x550;
    gSaveIntCnt2 = 0x20;

    test_haunted();
    haunted = 0x550;
}

/*  Do arith ops with the result & a const < 0x100.  This will cause the
 *  optimiser to drop the truncate and we should detect that by seeing an offset
 *  on the src or dst reg */
void t_add_mixed (void)
{
    unsigned char x = (gSaveIntCnt2 + haunted) & 0x20;
    test_execute (__func__, x == 0x20);
}

void t_sub_mixed (void)
{
    unsigned char x = (haunted - gSaveIntCnt2) & 0x20;
    test_execute (__func__, x == 0x20);
}

/*  And with a larger mask should prevent the optimiser from dropping the extend
 *  so we need to check we don't add one in error */
void t_sub_mixed_large_mask (void)
{
    int x = (haunted - gSaveIntCnt2) & 0x120;
    test_execute (__func__, x == 0x120);
}

void t_sub_mixed_r (void)
{
    unsigned char x = (gSaveIntCnt2 - haunted) & 0x60;
    test_execute (__func__, x == 0x40);
}

void t_xor_mixed (void)
{
    unsigned char x = (gSaveIntCnt2 ^ haunted) & 0x60;
    test_execute (__func__, x == 0x60);
}

void t_int_array (void)
{
    int toggleval = (hauntedIntArray[1] + gSaveIntCnt2) & 0x003f;
    if (toggleval == 0x20)
    {
        haunted = 0;
    }
    else if (toggleval == 0)
    {
        haunted = 1;
    }

    test_execute (__func__, haunted == 0);
}

void t_char_array (void)
{
    int toggleval = (hauntedCharArray[2] + gSaveIntCnt2) & 0x003f;
    if (toggleval == 0x20)
    {
        haunted = 0;
    }
    else if (toggleval == 0)
    {
        haunted = 1;
    }

    test_execute (__func__, haunted == 0);
}

/*  Add to a static long.  This will generate offsets of +/- 2 which are NOT
 *  byte offsets that should be corrected */

struct
{
    long l;
    char c[3];
    int i[2];
} str = { 0x12345678, { 0x11,0x22,0x33 }, { 0x33, 0x55 }};

void t_long_add(void)
{
    str.l += (str.c[1]+str.i[1]) & 0x10;
    test_execute (__func__, str.l == 0x12345688);
}

TESTFUNC tests[] = 
{
    t_bitwise_replace,
    t_mixed_params,
    t_cmp_sub,
    t_whoded,
    t_fptr_mixed,
    t_add_subreg,
    t_add_mixed,
    t_sub_mixed,
    t_sub_mixed_r,
    t_sub_mixed_large_mask,
    t_xor_mixed,
    t_int_array,
    t_char_array,
    t_long_add
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}

