#include "tap.h"

void test_array (const int k) {
   int v[k];
   int i;

   for (i = 0; i < k; i++) {
      v[i] = i;
   }

   printf ("%d\n", (int) sizeof (v));
   for (i = 0; i < k; i++) {
      printf ("%d\n", v[i]);
   }
}

int main (int argc, char *argv[]) {
    set_graphics(0);
    charset();
   printf ("Hello World!\n");
   test_array (5);
   printf ("done\n");
   while(1);
   return 0;
}

#if 0
void t_byte_array()
{
    int y;
    char x[1];
    // unsigned char x;
    int z;

    x[0]=0x55;
    // x=0xaa;
    y=4;
    z=2;
    int a=(x[0]==0x55);
    // int a=(x==0xaa);

    printf("# x=%X z=%d y=%d a=%d\n",
           (int)x[0], z, y, a);
           // (int)x[0], z, y, a);

    // test_execute (__func__, x[0]==0xaa);
    test_execute (__func__, x[0]==0x55);
}

TESTFUNC tests[] = 
{
    t_byte_array
};

int main (void)
{
    test_run (tests, 1);
}
#endif

