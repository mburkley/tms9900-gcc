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

static unsigned char array[] = { 5, 4, 3, 2, 1, 0 };

void t_array_loop (void)
{
    unsigned char i;

    ss_z=0;
    for (i=0; i < 5; i++)
    {
        set_ss_xy(get_uc_add(0,11+2*i), array[5-i]+15);
        ss_z += 11+i*2 + ss_x + ss_y;
        dprintf("x=%d y=%d z=%d\n", ss_x, ss_y, ss_z);
    }

    test_execute (__func__, ss_z==235);
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
