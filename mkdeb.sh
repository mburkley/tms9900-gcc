#!/bin/bash

SOURCE=$1
ARCH=$2
DISTRO=$3

if [ -z $SOURCE ] ; then
   echo Error: Please specify a source directory
   exit
fi
if [ -z $ARCH ] ; then
   echo Error: Please specify architecture amd64 or arm64
   exit
fi
if [ -z $DISTRO ] ; then
   echo Error: Please specify distribution bullseye, bookworm, ...
   exit
fi

#  Each architecture has a separate directory in the deb directory
#  since each package is for one architecture only and so needs a
#  unique control file.  The distro is appended to the version number
#  so we can have the same version package in different distros

REVISION=`cat dev/gcc-4.4.0/gcc/REVISION`
sed -i "s/Version: .*/Version: $REVISION~$DISTRO/g" deb/$ARCH/DEBIAN/control

echo cleaning...
rm -r deb/$ARCH/opt
echo copying files...
mkdir -p deb/$ARCH/opt
mkdir -p deb/$ARCH/opt/tms9900-gcc
rsync -a $SOURCE/ deb/$ARCH/opt/tms9900-gcc

#  Build a package with the version and include the distro as part 
#  of the version after a tilde (~).  This is to allow the same
#  version of a package but for different distros to exist at the
#  same time in the pool directory

dpkg -b deb/$ARCH "tms9900-gcc_$REVISION~$DISTRO""_$ARCH.deb"
