#ifndef MATH_PRIVATE_H
#define MATH_PRIVATE_H

// adjusts the exponent and first digit of mantissa to positive and returns -1 if the double is negative or 1 if not
int d_adj_neg_pos (double *d);

// compares two floats
int d_compare (double a, double b);

#endif
