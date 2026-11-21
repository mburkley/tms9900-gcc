#!/bin/bash

GCC_DIR=$1
GCC_PATCH_VER=$2
BIN_DIR=$3
BIN_PATCH_VER=$4

if [ -z "$BIN_PATCH_VER" ] ; then
   echo "Error: Please specify source directory names (under dev/) and patch"
   echo "version numbers for both gcc and binutils"
   echo
   echo "Usage: mkpatches.sh <gcc-dir> <gcc-patch-ver> <binutils-dir> <binutils-patch-ver>"
   echo "  e.g.: mkpatches.sh gcc-14.2.0 1.0 binutils-2.44 1.0"
   exit 1
fi

echo `date +%Y%m%d` > dev/$GCC_DIR/gcc/DATESTAMP
echo "TMS9900 patch" > dev/$GCC_DIR/gcc/DEV-PHASE
echo "$GCC_PATCH_VER" > dev/$GCC_DIR/gcc/REVISION

cd dev
diff -ru $GCC_DIR-orig $GCC_DIR | grep -v "Only in $GCC_DIR" > ../$GCC_DIR-tms9900-$GCC_PATCH_VER.patch
diff -ru $BIN_DIR-orig $BIN_DIR | grep -v "Only in $BIN_DIR" > ../$BIN_DIR-tms9900-$BIN_PATCH_VER.patch
cd ..

echo Done.  Now run install.sh to build gcc
