#!/bin/bash

SOURCE=$1
ARCH=$2
if [ -z $SOURCE ] ; then
   echo Error: Please specify a source directory
   exit
fi
if [ -z $ARCH ] ; then
   echo Error: Please specify architecture
   exit
fi
REVISION=`cat dev/gcc-4.4.0/gcc/REVISION`
echo copying files...
mkdir -p deb/$ARCH/opt
mkdir -p deb/$ARCH/opt/tms9900-gcc
rsync -a $SOURCE/ deb/$ARCH/opt/tms9900-gcc
sed -i "s/Version: .*/Version: $REVISION/g" deb/$ARCH/DEBIAN/control
dpkg -b deb/$ARCH "tms9900-gcc_$REVISION""_$ARCH.deb"
