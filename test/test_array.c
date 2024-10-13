#include "tap.h"
#include "params.h"

char a[4];

void t_array (void)
{
    int i;
    for (i = 0; i < 4; i++)
        ++a[i];

    test_execute (__func__, a[2]==1);
}

static unsigned char array[] = { 7, 11, 5, 19, 23 };

void t_array_loop (void)
{
    unsigned char i;

    for (i=0; i < 5; i++)
    {
        set_ss_xy(get_uc_add(0,11+2*i), array[5-i]+15);
        ss_x = 11+i*2;
    }
}

TESTFUNC tests[] = 
{
    t_array,
    t_array_loop
};

#define TEST_COUNT (sizeof (tests) / sizeof (TESTFUNC))

int main(void)
{
    test_run (tests, TEST_COUNT);

    return 0;
}
