#!/bin/bash

VERSION_GCC=$1
VERSION_BIN=$2
if [ -z $VERSION_BIN ] ; then
   echo Error: Please specify version numbers in the form x.y[.z] for both gcc and binutils
   exit
fi

echo `date +%Y%m%d` > dev/gcc-4.4.0/gcc/DATESTAMP
echo "TMS9900 patch" > dev/gcc-4.4.0/gcc/DEV-PHASE
echo "$VERSION_GCC" > dev/gcc-4.4.0/gcc/REVISION

cd dev
diff -ru gcc-4.4.0-orig gcc-4.4.0 | grep -v "Only in gcc-4.4.0" > ../gcc-4.4.0-tms9900-$VERSION_GCC.patch
diff -ru binutils-2.19.1-orig binutils-2.19.1 | grep -v "Only in binutils-2.19.1" > ../binutils-2.19.1-tms9900-$VERSION_BIN.patch
cd ..

echo Done.  Now run install.sh to build gcc
