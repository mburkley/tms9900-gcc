#!/bin/bash
#
#  This script builds a debian package repository locally and then mirrors it to an
#  ftp server.  Parameter 1 must be username:password for the ftp site.
#  Parameter 2 is optional and should be ??
#
USERPASS=$1
if [ -z $USERPASS ] ; then
   echo Error: Please provide username:password
   exit
fi
# dpkg-scanpackages -m . gt Packages
PKGS=\
tms9900-gcc_1.30_amd64_bullseye.deb \
tms9900-gcc_1.30_amd64.deb \
tms9900-gcc_1.31_arm64.deb
for p in $PKGS; do reprepro -b repo includedeb bookworm $PKG_X64_BULL
reprepro -b repo includedeb bookworm $PKG_X64
reprepro -b repo includedeb bookworm $PKG_ARM
# ftp -u ftp://$USERPASS@ti99ers.com/mburkley.pub repo/mburkley.pub
# ./dists/bookworm:
DIR=dists/bookworm
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/InRelease repo/$DIR/InRelease
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Release repo/$DIR/Release
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Release.gpg repo/$DIR/Release.gpg
# ./dists/bookworm/main/binary-amd64:
DIR=dists/bookworm/main/binary-amd64
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Packages repo/$DIR/Packages
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Packages.gz repo/$DIR/Packages.gz
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Release repo/$DIR/Release
DIR=dists/bookworm/main/binary-arm64
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Packages repo/$DIR/Packages
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Packages.gz repo/$DIR/Packages.gz
ftp -u ftp://$USERPASS@ti99ers.com/$DIR/Release repo/$DIR/Release

# ./pool/main/t/tms9900-gcc:
DIR=pool/main/t/tms9900-gcc
# ftp -u ftp://$USERPASS@ti99ers.com/$DIR/$PKG_X64 $PKG_X64
# ftp -u ftp://$USERPASS@ti99ers.com/$DIR/$PKG_ARM $PKG_ARM
