/*
 * Copyright (c) 2004-2024 Mark Burkley.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 *  Some resuable parsing functions.
 */

#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "parse.h"

/*  Safely parses a value from a string.  Value can be hex, decimal, octal, etc.
 *  Returns true if parsed successfully.
 */
bool parseValue (char *s, int *result)
{
    int base = 0;

    /*  If TI hex notation (>) is used or if x on its own is used then force
     *  base 16.  Otherwise let strtoul figure it out.
     */
    if (s[0] == '>' || s[0] == 'x')
    {
        s++;
        base = 16;
    }

    /*  Set errno to zero first and check it afterwards.  Slight safer than checking the
     *  return value
     */
    errno = 0;
    unsigned long conversion = strtoul (s, NULL, base);
    if (errno != 0)
        return false;

    *result = (int) conversion;
    return true;
}

void parseRemoveArg (int *argc, char **argv)
{
    for (int i = 1; i < *argc; i++)
        argv[i] = argv[i+1];

    (*argc)--;
}

/*  Break a line into word delimited by white space.  Destructive and maintains
 *  pointers to within line.  Ensure memory is not freed until all processing of
 *  args is finished.
 */
int parseLine (char *line, char *argv[])
{
    char *pos;
    int argc = 0;

    while ((pos = strtok (line, " \n\t")) != NULL)
    {
        argv[argc++] = pos;
        line = NULL;
    }

    return argc;
}

