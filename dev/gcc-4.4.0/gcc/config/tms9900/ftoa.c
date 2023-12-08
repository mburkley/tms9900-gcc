
/* ftoa.c
   This method converts a TI float to an ascii representation of a float.  It
   doesn't create very pretty output, but that doesn't matter, it's only for internal
   use.

   change log:
   7-DEC-2023 MGB initial version
*/

void tireal_ftoa (unsigned char *d, char *p)
{
    short first = d[0] << 8 | d[1];
    int exp;
    int i;
    if (first < 0)
    {
        *p++='-';
        first =- first;
    }

    exp = first >> 8;
    first &= 0xff;

    if (first>9) *p++=(first/10) + '0';
    *p++=(first%10)+'0';
    *p++='.';
    for (i = 2; i < 8; i++)
    {
        *p++=(d[i]/10)+'0';
        *p++=(d[i]%10)+'0';
    }

    exp -= 0x40;
    exp*=2;

    if (exp != 0)
    {
        *p++='E';

        if (exp<0)
        {
            *p++='-';
            exp=-exp;
        }

        if (exp>9)
        {
            *p++=(exp/10)+'0';
            exp/=10;
        }
        *p++=exp+'0';
    }
    *p=0;
}

