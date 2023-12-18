#!/bin/bash

SOURCE=$1
if [ -z $SOURCE ] ; then
   echo Error: Please specify a source directory
   exit
fi
REVISION=`cat dev/gcc-4.4.0/gcc/REVISION`
echo copying files...
rsync -a $SOURCE/ deb/opt/tms9900-gcc
sed -i "s/Version: .*/Version: $REVISION/g" deb/DEBIAN/control
dpkg -b deb tms9900-gcc-$REVISION.deb
