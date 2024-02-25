#!/bin/bash
INCLUDE=-I../libti99i
INCLUDE="$INCLUDE -I../vgmcomp2/Players/libtivgm2"
INCLUDE="$INCLUDE -I../ghostbusters/include"
CFLAGS="$2 $3 $4 $5 $6 -Wall -da -std=c99 -fomit-frame-pointer $INCLUDE"
../dev/gcc-4.4.0/build/gcc/cc1 $CFLAGS < $1
# > gcc4.log
