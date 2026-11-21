/* d_adj_neg_pos.c
   This method performs absolute value on the incoming double and indicates whether it was positive or negative.
   06/23/2023 mrvan initial version
*/

#include "math_private.h"

int d_adj_neg_pos (double *d) {
   char *p = (char *) d;
   int np = 1;
   if (*p < 0) {
      *p = -(*p) - 1;
      np = -1;
      p++;
      *p = -(*p);
   }
   return np;
}
