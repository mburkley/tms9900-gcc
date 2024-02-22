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

The branch **main** in the repo contains The latest scripts and patch files. The patch files are:
* binutils-2.19.1-tms9900-1.10.patch
* gcc-4.4.0-tms9900-1.30.patch

Patch files are cumulative.  The latest version of the patch file includes all
previous updates as well.  Only the latest patch file is stored in the repo.
The install script will find the latest patch and apply it.

User download and install
-------------------------
For users that just want to use the compiler, please follow these steps to clone
the main branch and install build tools. You will need a recent version of gcc
installed.

> git clone [git@github.com:mburkley/tms9900-gcc.git][5]

> sudo apt install build-essential libgmp-dev libmpfr-dev

> cd tms900-gcc

> ./install.sh <target>

 where <target> is your desired install directory
(/usr/local/tms9900-gcc for example)

Release Notes
-------------
binutils patch 1.10

* No changes to 1.9 but includes the multi-def patch to avoid compile errors relating to "do_wide"

gcc patch 1.30
* Pass constants as wides to force_const_mem to avoid assert in combine.c:do_SUBST
* Added calls to correct byte order on all byte and word arith and move
* Changed inline debug to dump entire insn not just operands
* Removed wrongly associative constraints on subtract
* Added separate reg constraints to addhi3, andhi3, subhi3 to allow longer lengths for subreg offset fixes
* Added more unit tests
* Removed constraints in movqi - causes assert in reload

gcc patch 1.29
* Add function to swpb before or after MOV[B] if subreg offset seen
* Fix operand count for one_cmpl ops
* Changed compare insns "cmphi" and "cmpqi" to be insn_and_split
* Added peepholes to remove SRL Rx,8 ; SWPB
* Added R12 thru R15 to save regs to improve general reg availability
* Added literal constants for movqi and movhi to avoid LI to a scratch reg
* Removed sub_const_hi and associated peepholes
* Removed NEG from subtract operations that had reversed operands
* Added debugs to peepholes to find which ones are actually used

gcc patch 1.28
* Improve shift operations by moving shift count to R0
* Use R0 as a clobber in compare and other instructions
* Fix issue in mov QI where truncate from HI was missed
* Fix compare to zero by mov'ing to R0 instead of dest
* Added improved 32-bit divide and multiply libgcc functions from mrvan

gcc patch 1.27
* Revert SP to R10 and BP to R9
* Add case to andhi3 to fix missed byte extend (bug manifested in libti99/vdp_bmcolor.c)
* Replace SRL with SWPB in trunc
* Added macros  __TMS9900_PATCH_MAJOR__ and  __TMS9900_PATCH_MINOR__ to query patch level

gcc patch 1.26
* Fix the regression where R11 is not saved
* Make R15 fixed so compiler does not try to save it

gcc patch 1.25
* Uses TI float format internally for floats and doubles
* libgcc now includes floating point operations to call console ROM routines
    * **NOTE** Do not include libgcc if you don't want any dependency on console ROM
* Moves stack pointer from R10 to R15 and base pointer from R9 to R14
    * This change allows R1-R10 to be used as general regs which improves
performance
    * **NOTE** Check your crt0.asm file to ensure it is setting up R15 as the
    * stack pointer and not R10 which was used previously
* Fixes byte compares
* Fixes immediate AND and OR operations on byte values
* Fixes shift of long by a variable
* Removes insns relating to 32-bit arithmetic and shift
    * The compiler emits 16-bit instructions to perform these
* Includes more unit tests for bytes, shorts, longs and floats

gcc patch 1.24
* Fixes OR operation clearing the value of a byte
* Fixes initialisation of a long
* Fixes string init with a value longer than 1024 bytes

gcc patch 1.23 
* Fixes stack frame saved register and local variable space allocation

Implementation Notes
====================

Compiler switches
-----------------
For optimised builds use **-O2** for speed or **-Os** for size as usual.

**-fno-function-cse** is receommended to prevent loading label addresses in to
registers before branch

**-fomit-frame-pointer** is recommended if using inline assembly that uses all
registers as otherwise gcc will complain that R9 is not available.  This
shouldn't be needed if building optimised for size since gcc omits frame
pointers anyway in that case.

**-minline_rtl** will add RTL insn dumps into the assembly output.

Integer types supported
-----------------------

Arithmetic (+,-,*,/,%), shift (<<,>>) and bitwise (&,|,^) operations are supported
as normal for any of the following supported C-types which can declared as either signed or unsigned:
* char - 8-bits
* short - 16-bits
* int - 16-bits
* long - 32-bits

The following types are not supported:
* long long - 64-bits

Register allocation
-------------------
The latest version of tms9900-gcc uses the following register allocations:
* R0 - reserved for bit shift operations and internal use
* R9 - Base pointer for stack frames
* R10 - Stack pointer
* R11 - Function return pointer
* R12 - Not currently used but marked as available for static chain pointer

The compiler will use R1-R8 and R12-R15 as general registers.  Parameters are
passed in registers starting from R1.

The compiler does not have any ability to emit any CRU instructions
(STCR,LDCR,SB,SBO,TB).  Inline ASM code that uses CRU instructions should save
R12.

The compiler no longer saves R13,R14 and R15 when entering a function.  If you are writing code that
will be invoked through a BLWP instruction, then you must save these regisers
before calling code that was generated by this compiler.

**NOTE** There is no stack overflow detection.  If using RAM at >2000 for BSS
and stack from >4000
make sure there is enough space for the stack grow downward without colliding
with the data.

Workspace area
--------------

By default code built with this compiler (e.g. libti99) has used >8300 as the
workspace address.  Please note that if you are using the console ROM routines
then these routines will use >8300 as a scratchpad area and will corrupt registers.  The
space at either >8380 or >83A0 can be used instead if using console ROM routines
and not running GPL.

Floating point types
--------------------

The following floating point types are supported:
* float - 64-bits
* double - 64-bits

By default, gcc for tms9900 will emit calls to floating point arithmetic
functions with parameters encoded in [TI99 floating point format][4] (64-bit/8-byte radix
100).  The libgcc library contains calls to console ROM routines to perform
arithmetic operations.  If your code does not contains any float or double
declarations, then no links to floating point libraries will be made.

If you don't use floats or doubles in your code, then no action is required.

If you do use floats or doubles and you want to use console ROM routines for
speed or to save space, then add -lgcc to your link command line (you may need
to provide the path to libgcc).

If you want to use floats but don't want to use console ROM routines, then you
will need to link to an alternative floating point emulation library.  Emulation
libaries typically expect
IEEE radix-2 floats, so use the compiler switch **-mno-ti99-float** to disable
floats to be formatted as TI floats.

**NOTE:** Don't link an object file that uses floats against libgcc if you are not using TI99
floating point format.  The results will be unpredictable.


Compiler developers
-------------------
For users that want to add new features or bug fixes, the directory **dev**
contains original and modified files to configure gcc for the tms9900.  Be sure
to make edits only to files in the dev directory, not the build directory, as
build is not under version control.

The dev directory contains only the files that have changed to support this
configuration.  The directory **dev/gcc-4.4.0-orig** contains the original versions
of any changed files or a zero-length file for any new files (e.g. tms9900.md).
To build a development version of gcc, you will need to either untar gcc over
**dev/gcc-4.4.0** and then do a git checkout *or* move the dev directory contents aside
and then copy them over the untar'd gcc sources *or* reapply the patches to the
untar'd sources.

If you are adding a new file under dev/gcc-4.4.0, make sure to touch a copy of the same
file in dev/gcc-4.4.0-orig and add both to git.  You will need to use **git -f
add** since the **dev** directory is ignored by default to avoid listing all
source files in gcc.

Once you have finished your changes, commit the changed files (not the entire
source tree) in dev/gcc-4.4.0 and recreate the patch files from within the build
directory using the command **./mkpatches.sh**:

Open issues
-----------

Open issues are tracked [here][6]

[1]: https://forums.atariage.com/topic/164295-gcc-for-the-ti/
[2]: https://ftp.gnu.org/gnu/binutils/binutils-2.19.1.tar.bz2
[3]: https://ftp.gnu.org/gnu/gcc/gcc-4.4.0/gcc-4.4.0.tar.gz
[4]: https://www.unige.ch/medecine/nouspikel/ti99/reals.htm
[5]: git@github.com:mburkley/tms9900-gcc.git
[6]: https://github.com/mburkley/tms9900-gcc/issues
