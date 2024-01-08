/* ulongdivmod.c
   These methods provide unsigned long division and modulus.
   change log:
   12/26/2023 initial version
*/

// #include <ulongdivmod.h>

unsigned long numerator_u32_cache = 0;
unsigned long denominator_u32_cache = 0;
volatile unsigned long quotient_u32_cache;
volatile unsigned long remainder_u32_cache;

void divmod_u32 (long numerator, long denominator) {

   unsigned long t;

   quotient_u32_cache  = 0;
   remainder_u32_cache = 0;

   int i;
   for (i = 31 ; i >= 0; i--) {
      remainder_u32_cache = remainder_u32_cache << 1;
      t                   = (long) 1 << i;
      t                   = t & numerator;
      t                   = t >> i;
      remainder_u32_cache = remainder_u32_cache | t;
      if (remainder_u32_cache >= denominator) {
         remainder_u32_cache = remainder_u32_cache - denominator;
         quotient_u32_cache  = quotient_u32_cache | ((long) 1 << i);
      }
   }
}

long __udivsi3 (unsigned long numerator, unsigned long denominator) {
   if (numerator != numerator_u32_cache || denominator != denominator_u32_cache) {
      numerator_u32_cache   = numerator;
      denominator_u32_cache = denominator;
      divmod_u32 (numerator_u32_cache, denominator_u32_cache);
   }
   return quotient_u32_cache;
}

unsigned long __umodsi3 (unsigned long numerator, unsigned long denominator) {
   if (numerator != numerator_u32_cache || denominator != denominator_u32_cache) {
      numerator_u32_cache   = numerator;
      denominator_u32_cache = denominator;
      divmod_u32 (numerator_u32_cache, denominator_u32_cache);
   }
   return remainder_u32_cache;
}
