#!/bin/bash

VERSION=$1
if [ -z $VERSION ] ; then
   echo Error: Please specify a version number x.y[.z]
   exit
fi

echo `date +%Y%m%d` > dev/gcc-4.4.0/gcc/DATESTAMP
echo "TMS9900 patch" > dev/gcc-4.4.0/gcc/DEV-PHASE
echo "$VERSION" > dev/gcc-4.4.0/gcc/REVISION

cd dev
diff -ru gcc-4.4.0-orig gcc-4.4.0 | grep -v "Only in gcc-4.4.0" > ../gcc-4.4.0-tms9900-$VERSION.patch
diff -ru binutils-2.19.1-orig binutils-2.19.1 | grep -v "Only in binutils-2.19.1" > ../binutils-2.19.1-tms9900-1.10.patch
cd ..

echo Done.  Now run install.sh to build gcc
