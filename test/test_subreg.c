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

TESTFUNC tests[] = 
{
    t_bitwise_replace,
    t_mixed_params,
    t_cmp_sub,
    t_whoded,
    t_fptr_mixed
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}

