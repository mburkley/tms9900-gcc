/* 06/23/2023 mrvan initial version
*/

#include "math_private.h"

// returns a value greater than zero a is strictly greater than b
int __gtdf2 (double a, double b) {
   return d_compare (a, b);
} 
