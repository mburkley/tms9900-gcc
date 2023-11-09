tms9900-gcc: TI 99 cross compiler backend for gcc
=================================================
This repo for the **gcc cross-compiler for TMS9900** was created from files posted
by insomnia and others to atariage [here][1].  It is based on [gcc-4.4.0][2] and
[binutils-2.19.1][3].

There have been a number of patches posted to the gcc-for-the-ti thread and this
repo plans to consolidate these patches into one consistent package.

In order to avoid having to commit the entire codebase for gcc and binutils
(over 200MB), this repo maintains only the differences required to enable
tms9900 support.  This differences are much smaller.

Patch files are archived in the main branch which are used by an install script
to create modified sources for gcc and binutils.

The branches in the repo are:

 * [main]: The latest scripts and patch files
 * [gcc-4.4.0]: Unmodified source code files for gcc-4.4.0 and binutils-2.19.1.
This branch is a reference for developers to more easily create new patch
files.  It contains only the files that are modified by the patches.
 * [patched-files]: branch containing patched versions of source files

User download and install
-------------------------
For users that just want to use the compiler, please follow these steps to clone
the main branch and install build tools. You will need a recent version of gcc
installed.

 * git clone git@github.com:mburkley/tms9900-gcc.git
 * apt install build-essential libgmp-dev libmpfr-dev
 * cd tms900-gcc
 * ./install.sh <target>

 where <target> is your desired install directory
(/usr/local/tms9900-gcc for example)

Compiler developers
-------------------
For users that want to add new features or bug fixes, the following steps apply.
The steps to create new patch files are a bit manual for now.

If you want to create a change request for the compiler or the binutils, create
a new branch based on main and apply the patches using install.  
This will allow you to more
easily track changes against the latest source code.
You will also
need a copy of the unmodified gcc/binutils source code to generate the patch.

Get a copy of the relevant unmodified sources:

 * cd tms9900-gcc
 * git checkout gcc-4.4.0
 * mv build/gcc-4.4.0 build/gcc-orig
 * mv build/binutils-2.19.1 build/binutils-orig

Create a new branch with full buildable sources:

 * git checkout -b my-branch
 * git rebase main
 * ./install.sh

Once you have finished your changes, commit the changed files (not the entire
source tree) and recreate the patch files from within the build directory:

 * diff -ru gcc-orig gcc-4.4.0 | grep -v "Only in gcc-4.4.0" > ../gcc-4.4.0-tms9900-1.20.patch
 * diff -ru binutils-orig binutils-2.19.1 | grep -v "Only in binutils-2.19.1" > ../binutils-2.19.1-tms9900-1.9.patch

Add the two new patches to git, push the branch and create a PR 


[1]: https://forums.atariage.com/topic/164295-gcc-for-the-ti/
[2]: https://ftp.gnu.org/gnu/binutils/binutils-2.19.1.tar.bz2
[3]: https://ftp.gnu.org/gnu/gcc/gcc-4.4.0/gcc-4.4.0.tar.gz

