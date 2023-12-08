#include "math_private.h"

// returns a value greater than or equal to zero if a is greater than or equal to b
int __gedf2 (double a, double b) {
   return d_compare (a, b); 
}
