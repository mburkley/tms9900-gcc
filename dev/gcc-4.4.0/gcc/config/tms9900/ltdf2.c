#include <math_private.h>

// returns a value less than zero if a is strictly less than b
int __ltdf2 (double a, double b) {
   return d_compare (a, b); 
}
