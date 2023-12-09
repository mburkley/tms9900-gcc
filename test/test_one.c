#include "tap.h"

void t_int32_sla_var()
{
    int32_t x = 0x11224488;
    int32_t y;

    int z=4;
    y = x<<z;
    unsigned short lo=y;
    unsigned short hi=y>>16;
    printf("# y=>%04X >%04X\n",hi,lo);
    test_execute (__func__, y== 0x12244880);
}

TESTFUNC tests[] = 
{
    t_int32_sla_var
};

int main (void)
{
    test_run (tests, 1);
}

