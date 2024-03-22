/* double.c
   This file includes double math functions that the GCC compiler requires.
   06/23/2023 mrvan initial version
   08/14/2023 mrvan removed methods not associated with the FAC and ARG to separate files
*/

#define FAC 0x834A
#define ARG 0x835C
#define FAC_PTR ((volatile double*)FAC)
#define ARG_PTR ((volatile double*)ARG)

#define FADD_AUGEND FAC_PTR
#define FADD_ADDEND ARG_PTR
#define FADD_SUM    FAC_PTR

#define FSUB_MINUEND    ARG_PTR
#define FSUB_SUBTRAHEND FAC_PTR
#define FSUB_DIFFERENCE FAC_PTR

#define FMUL_MULTIPLICAND ARG_PTR
#define FMUL_MULTIPLIER   FAC_PTR
#define FMUL_PRODUCT      FAC_PTR

#define FDIV_DIVIDEND FAC_PTR
#define FDIV_DIVISOR  ARG_PTR
#define FDIV_QUOTIENT FAC_PTR

// double addition -- method normally included in GCC lib
double __adddf3 (double a, double b) {

   // use the ti99's ROM method

   // adds a and b
   *FADD_AUGEND = a;
   *FADD_ADDEND = b;

  __asm__(
    "lwpi >83E0\n\t"
    "bl @>0D80\n\t"
    "lwpi >83A0\n\t"
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
    "lwpi >83A0\n\t"
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
    "lwpi >83A0\n\t"
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
    "lwpi >83A0\n\t"
  );

  return *FDIV_QUOTIENT;
}
