#!/bin/bash
DEVPHASE=`cat dev/gcc-4.4.0/gcc/DEV-PHASE` >> mkdeb.sh
VERSION=${DEVPHASE:14}
rsync -a tms9900-gcc deb/opt
dpkg -b deb tms9900-gcc-$VERSION.deb
