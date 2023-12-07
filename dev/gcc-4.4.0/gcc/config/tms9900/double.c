/* double.c
   This file includes double math functions that the GCC compiler requires.
   06/23/2023 mrvan initial version
   08/14/2023 mrvan removed methods not associated with the FAC and ARG to separate files
*/

#include <math_private.h>

#define FAC 0x834A
#define ARG 0x835C;

double *FADD_AUGEND       = (double *)FAC;
double *FADD_ADDEND       = (double *)ARG;
double *FADD_SUM          = (double *)FAC;

double *FSUB_MINUEND      = (double *)ARG;
double *FSUB_SUBTRAHEND   = (double *)FAC;
double *FSUB_DIFFERENCE   = (double *)FAC;

double *FMUL_MULTIPLICAND = (double *)ARG;
double *FMUL_MULTIPLIER   = (double *)FAC;
double *FMUL_PRODUCT      = (double *)FAC;

double *FDIV_DIVIDEND     = (double *)FAC;
double *FDIV_DIVISOR      = (double *)ARG;
double *FDIV_QUOTIENT     = (double *)FAC;

char *EXP                 = (char *)0x8376;
unsigned char *SIGN       = (unsigned char *)0x8375; // >00 = positive, >FF = negative
unsigned char *OVFL       = (unsigned char *)0x8354; // >01 if overflow

// double addition -- method normally included in GCC lib
double __adddf3 (double a, double b) {

   // use the ti99's ROM method

   // adds a and b
   *FADD_AUGEND = a;
   *FADD_ADDEND = b;

  __asm__(
    "lwpi >83E0\n\t"
    "bl @>0D80\n\t"
    "lwpi >8300\n\t"
  );

  return *FADD_SUM;
}

// double subtraction -- method normally included in GCC lib
double __subdf3 (double a, double b) {

   // use the ti99's ROM method

   // subtract b from a
   *FSUB_MINUEND    = a;
   *FSUB_SUBTRAHEND = b;

  __asm__(
    "lwpi >83E0\n\t"
    "bl @>0D7C\n\t"
    "lwpi >8300\n\t"
  );

  return *FSUB_DIFFERENCE;
}

// double multiplication -- method normally included in GCC lib
double __muldf3 (double a, double b) {

   // use the ti99's ROM method

   // multiply a and b
   *FMUL_MULTIPLICAND = a;
   *FMUL_MULTIPLIER   = b;

  __asm__(
    "lwpi >83E0\n\t"
    "bl @>0E88\n\t"
    "lwpi >8300\n\t"
  );

  return *FMUL_PRODUCT;
}

// double division -- method normally included in GCC lib
double __divdf3 (double a, double b) {

   // use the ti99's ROM method

   // divide b by a
   *FDIV_DIVIDEND = b;
   *FDIV_DIVISOR  = a;

  __asm__(
    "lwpi >83E0\n\t"
    "bl @>0FF4\n\t"
    "lwpi >8300\n\t"
  );

  return *FDIV_QUOTIENT;
}
