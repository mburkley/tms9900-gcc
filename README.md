tms9900-gcc: TI 99 cross compiler backend for gcc
=================================================
This repo for the **gcc cross-compiler for TMS9900** was created from files posted
by insomnia and others to atariage [here][1].  It is based on [gcc-4.4.0][2] and
[binutils-2.19.1][3].

There have been a number of patches posted to the gcc-for-the-ti thread and this
repo consolidates these patches into one consistent package.

In order to avoid having to commit the entire codebase for gcc and binutils
(over 200MB), this repo maintains only the differences required to enable
tms9900 support.  This differences are much smaller.

Patch files are archived in the main branch which are used by an install script
to create modified sources for gcc and binutils.

The branch "main" in the repo contains The latest scripts and patch files. The patch files are:
* binutils-2.19.1-tms9900-1.10.patch
* gcc-4.4.0-tms9900-1.25.patch

Patch files are cumulative.  The latest version of the patch file includes all
previous updates as well.  Only the latest patch file is stored in the repo.
The install script will find the latest patch and apply it.

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

Release Notes
-------------
binutils patch 1.10

* No changes to 1.9 but includes the multi-def patch to avoid compile errors relating to "do_wide"

gcc patch 1.25

* Uses TI float format internally for floats and doubles
* libgcc now includes floating point operations to call console ROM routines
** Do not include libgcc if you don't want any dependency on console ROM
* Moves stack pointer from R10 to R15 and base pointer from R9 to R14
** This change allows R1-R10 to be used as general regs which improves
performance
* Fixes byte compares
* Fixes immediate AND and OR operations on byte values
* Fixes shift of long by a variable
* Removes insns relating to 32-bit arithmetic and shift
* Includes more unit tests for bytes, shorts, longs and floats

gcc patch 1.24
* Fixes OR operation clearing the value of a byte
* Fixes initialisation of a long
* Fixes string init with a value longer than 1024 bytes

gcc patch 1.23 
* Fixes stack frame saved register and local variable space allocation

Compiler developers
-------------------
For users that want to add new features or bug fixes, modify files and build in
the dev directory not the build directory.

If you want to create a change request for the compiler or the binutils, create
a new branch based on main and apply the patches using install.  
This will allow you to more
easily track changes against the latest source code.

Once you have finished your changes, commit the changed files (not the entire
source tree) and recreate the patch files from within the build directory:

 * diff -ru gcc-4.4.0-orig gcc-4.4.0 | grep -v "Only in gcc-4.4.0" > ../gcc-4.4.0-tms9900-1.22.patch
 * diff -ru binutils-2.19.1-orig binutils-2.19.1 | grep -v "Only in binutils-2.19.1" > ../binutils-2.19.1-tms9900-1.10.patch

Add the two new patches to git, push the branch and create a PR 


[1]: https://forums.atariage.com/topic/164295-gcc-for-the-ti/
[2]: https://ftp.gnu.org/gnu/binutils/binutils-2.19.1.tar.bz2
[3]: https://ftp.gnu.org/gnu/gcc/gcc-4.4.0/gcc-4.4.0.tar.gz

