/* longdivmod.c
   These methods provide long division and moduls.
   change log:
   12/26/2023 initial version
*/

// #include <longdivmod.h>

long numerator_s32_cache = 0;
long denominator_s32_cache = 0; 
volatile long quotient_s32_cache;
volatile long remainder_s32_cache;

void divmod_s32 (long numerator, long denominator) {

   long sign = 1;
   long t;

   quotient_s32_cache  = 0;
   remainder_s32_cache = 0;

   if (numerator < 0) {
      sign      = sign * -1;
      numerator = -numerator;
   }

   if (denominator < 0) {
      sign        = sign * -1;
      denominator = -denominator;
   }

   int i;
   for (i = 31 ; i >= 0; i--) {
      remainder_s32_cache = remainder_s32_cache << 1;
      t                   = (long) 1 << i;
      t                   = t & numerator; 
      t                   = t >> i;
      remainder_s32_cache = remainder_s32_cache | t;
      if (remainder_s32_cache >= denominator) {
         remainder_s32_cache = remainder_s32_cache - denominator;
         quotient_s32_cache  = quotient_s32_cache | ((long) 1 << i);
      }
   }

   quotient_s32_cache = quotient_s32_cache * sign;
}

long __divsi3 (long numerator, long denominator) {
   if (numerator != numerator_s32_cache || denominator != denominator_s32_cache) {
      numerator_s32_cache   = numerator;
      denominator_s32_cache = denominator;
      divmod_s32 (numerator_s32_cache, denominator_s32_cache);
   }
   return quotient_s32_cache;
}

long __modsi3 (long numerator, long denominator) {
   if (numerator != numerator_s32_cache || denominator != denominator_s32_cache) {
      numerator_s32_cache   = numerator;
      denominator_s32_cache = denominator;
      divmod_s32 (numerator_s32_cache, denominator_s32_cache);
   }
   return remainder_s32_cache;
}
