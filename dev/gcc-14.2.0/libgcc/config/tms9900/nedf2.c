/* returns nonzero if two double values are not equal */
extern int __eqdf2 (double a, double b);

int __nedf2 (double a, double b) {
   return __eqdf2 (a, b);
}
