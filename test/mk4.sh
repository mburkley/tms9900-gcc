#!/bin/bash
CFLAGS="$2 $3 $4 $5 $6 -Wall -da -I ../libti99 -std=c99"
../dev/gcc-4.4.0/build/gcc/cc1 $CFLAGS < $1
# > gcc4.log
