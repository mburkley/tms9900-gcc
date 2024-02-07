#!/bin/bash
CFLAGS="$2 -da -I ../libti99/"
../dev/gcc-13.2.0/build/gcc/cc1 $CFLAGS -o gcc13.s  $1 
#> gcc13.log
