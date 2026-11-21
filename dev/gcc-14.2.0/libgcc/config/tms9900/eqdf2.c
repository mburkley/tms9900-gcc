/* 06/23/2023 mrvan initial version
*/
// returns 0 if two double values are equal
int __eqdf2 (double a, double b) {
   char *ap = (char *) &a;
   char *bp = (char *) &b;
   int cmp = 0;
   int i;
   for (i = 0; i < 8; i++) {
      cmp |= *ap != *bp;
      ap++;
      bp++;
   }
   return cmp;
}
