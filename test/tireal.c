#include <stdio.h>
#include <stdlib.h>

#include "decContext.h"
#include "decNumber.h"
// #include "decDouble.h"

static decContext cont =
{
    14,
    63,
    -63,
    0,
    0,
    0,
    0
};

#if 0
typedef union
{
    unsigned char b[8];
    unsigned long l[2];
}
tireal;

real *encode (int sign, int exp, long mant)
{
#if 0
static void
tms9900_encode_real (const struct real_format *fmt, long *buf,
		    const REAL_VALUE_TYPE *r)
#endif
{
  char s[24];
  decimal64ToString() // ??
    real r;

    /* convert exp b2 to exp b10 */

    /* convert digs b2 to digs b10 */

    if (sign)

}

void decode (real r)
{
}
#endif

// void dump (real *r)
#ifndef DECDOUBLE_Bytes
#define DECDOUBLE_Bytes 8
#endif

void dump (decNumber *r)
{
  unsigned char *c = r->bytes;
  int i;

  for (i = 0; i < DECDOUBLE_Bytes * sizeof(long); i++)
      printf (" %02X", c[i]);

  printf("\n");
}

int main (int argc, char *argv[])
{
    if (argc<2)
    {
        fprintf (stderr, "number param needed\n");
        exit(1);
    }
    // real r1;
    decNumber r;
    decNumberFromString(&r, argv[1], &cont);
    // r1.l[0]=r1.l[1]=0;
    // r1[0]=10;
    // r1[1]=42;
    // encode (0, 10, 42);
    // decode (r1);
}

