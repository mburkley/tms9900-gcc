#!/bin/bash
#
#  This script builds a debian package repository locally and then mirrors it to an
#  ftp server.  Parameter 1 must be username:password for the ftp site.
#  Parameter 2 is optional and should be ??
#
PKGS_BULLSEYE=tms9900-gcc*bullseye*.deb
PKGS_BOOKWORM=tms9900-gcc*bookworm*.deb
PKGS_TRIXIE=
echo adding to repo : $PKGS_BOOKWORM
for p in $PKGS_BOOKWORM ; do reprepro -b repo includedeb bookworm $p ; done
echo adding to repo : $PKGS_BULLSEYE
for p in $PKGS_BULLSEYE ; do reprepro -b repo includedeb bullseye $p ; done
echo adding to repo : $PKGS_TRIXIE
for p in $PKGS_TRIXIE ; do reprepro -b repo includedeb trixie $p ; done

