#include "math_private.h"

// returns a value less than or equal to zero if a is less than or equal to b
int __ledf2 (double a, double b) {
   return d_compare (a, b); 
}  
