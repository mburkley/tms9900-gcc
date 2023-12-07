#include <math_private.h>

// returns a value less than zero if a is strictly less than b, 0 if equal, 1 if a is strictly greater than b
int d_compare (double a, double b) { // , int lt_response, int gt_response, int eq_response) {
   int npa = 1;
   int npb = 1;           
   char *pa, *pb;
   int r;
   int i;

   npa = d_adj_neg_pos (&a);
   npb = d_adj_neg_pos (&b);
   
   if (npa < npb) {
      // r = lt_response;
      r = -1;
   } else if (npa > npb) {
      // r = gt_response;
      r = 1;
   } else {
      pa = (char *) &a;
      pb = (char *) &b;
      // r = eq_response;
      r = 0;
      for (i = 0; i < 8; i++) {
         // first test if they are unequal and if so determine how so, otherwise move on.
         // one comparison if equal, 1-2 if not.
         if (!(*pa == *pb)) {
            if (*pa < *pb) {
               // r = lt_response;
               r = -1;
               break;
            } else if (*pa > *pb) {
               // r = gt_response;
               r = 1;
               break;
            }
         }
         pa++;
         pb++;
      }
   }

   return r;
}
