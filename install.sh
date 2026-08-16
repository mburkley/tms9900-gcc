set +x
# Super simple patch and install script for GCC and Binutils


# Get latest file version
#   1) List files
#   2) Convert "filename-x.yy.patch" to "0000x 0000yy filename-x.yy.patch"
#   3) Convert to "000x 00yy filename-x.yy.patch"
#   4) Sort list
#   5) Convert to "filename-x.yy.patch"
#   6) Return last item
latest ()
{
  ls $1 | \
  sed 's/\(.*-\)\([0-9]\+\)\.\([0-9]\+\)\(.*patch\)/0000\2 0000\3 \1\2.\3\4/' |\
  sed 's/0*\([0-9]\{4\}\) /\1 /g' |\
  sort -n |\
  sed 's/.* //' |\
  tail -1
}


# Print error text if last command failed
check_result ()
{
  if [ ! $? == 0 ] ; then
    echo $1
    exit
  fi
}


# Check arguments
PREFIX=$1
if [ -z $PREFIX ] ; then
   echo Error: No output directory specified
   echo
   echo Usage:
   echo   install.sh DIRECTORY [VERSION]
   echo The output binaries will be installed at DIRECTORY/bin
   echo VERSION is optional: 1 for gcc-4.4.0/binutils-2.19.1, 2 for
   echo gcc-14.2.0/binutils-2.44.  If omitted, you will be prompted.
   exit
fi

# Pick which gcc/binutils pair to build.  Patches for more than one
# version can be present in this directory at once (a stable version
# and an in-development one), so we can't just always pick the patch
# with the highest revision number as "latest" any more.
VERSION_CHOICE=$2
if [ -z "$VERSION_CHOICE" ] ; then
  echo "Which version would you like to install?"
  echo "  1) gcc-4.4.0 / binutils-2.19.1 (stable)"
  echo "  2) gcc-14.2.0 / binutils-2.44 (in development)"
  printf "Enter 1 or 2 [1]: "
  read VERSION_CHOICE
  if [ -z "$VERSION_CHOICE" ] ; then
    VERSION_CHOICE=1
  fi
fi

case $VERSION_CHOICE in
  1)
    BINUTILS_GLOB="binutils-2.19.1-tms9900-*.patch"
    GCC_GLOB="gcc-4.4.0-tms9900-*.patch"
    GCC_CONFIGURE_EXTRA=""
    ;;
  2)
    BINUTILS_GLOB="binutils-2.44-tms9900-*.patch"
    GCC_GLOB="gcc-14.2.0-tms9900-*.patch"
    # Avoids a codegen ICE in libgcov-driver.c: this freestanding target
    # has no OS to write coverage files, and gcov support was never a
    # goal, so skip building it rather than chase the ICE.
    GCC_CONFIGURE_EXTRA="--disable-gcov"
    ;;
  *)
    echo "Error: Invalid version '$VERSION_CHOICE', expected 1 or 2"
    exit
    ;;
esac

# Base all following work on latest patch versions for the chosen pair
BINUTILS_PATCH=`latest $BINUTILS_GLOB`
GCC_PATCH=`latest $GCC_GLOB`

# Extract tool versions from patch name
BINUTILS_VERSION=`echo $BINUTILS_PATCH | sed "s/-tms9900.*//"`
GCC_VERSION=`     echo $GCC_PATCH      | sed "s/-tms9900.*//"`

# Compose name of source archive
BINUTILS_ARCHIVE="$BINUTILS_VERSION.tar.bz2"
GCC_ARCHIVE="$GCC_VERSION.tar.gz"

# Parallel build jobs: one per CPU core, so `make` actually uses all
# available cores instead of building single-threaded.
JOBS=`nproc 2>/dev/null`
if [ -z "$JOBS" ] ; then
  JOBS=1
fi

# Select download tool
if [ ! -z "`which wget`" ]; then
  WGET=wget
elif [ ! -z "`which curl`" ]; then
  WGET="curl -L -O"
fi

echo "Using these patches:"
echo "   BINUTILS: $BINUTILS_PATCH"
echo "   GCC: $GCC_PATCH"
echo

echo "=== Creating output directory ==="
if [ ! -d $PREFIX ] ; then
  mkdir -p $PREFIX
  check_result "=== Failed to create output directory ==="
fi

START=`pwd`
PREFIX=`cd $PREFIX; pwd`

echo "=== Getting Binutils sources ==="
if [ ! -f $BINUTILS_ARCHIVE ] ; then
  $WGET http://ftp.gnu.org/gnu/binutils/$BINUTILS_ARCHIVE
  check_result "=== Failed to get Binutils sources ==="
fi

echo "=== Getting GCC sources ==="
if [ ! -f $GCC_ARCHIVE ] ; then
  $WGET http://ftp.gnu.org/gnu/gcc/$GCC_VERSION/$GCC_ARCHIVE
  check_result "=== Failed to get GCC sources ==="
fi

echo "=== Make build directory ==="
if [ ! -d build ] ; then
  mkdir build
  check_result "=== Failed to create build directory ==="
fi
cd build

echo "=== Decompressing and patching Binutils sources ==="
if [ ! -d $BINUTILS_VERSION ] ; then
   bunzip2 -c ../$BINUTILS_ARCHIVE > binutils.tar && tar -xf binutils.tar && rm binutils.tar
   check_result "=== Failed to decompress Binutils sources ==="
fi
if [ ! -f .binutils_patched ] ; then
   cd $BINUTILS_VERSION
   patch -p1 < ../../$BINUTILS_PATCH
   check_result "=== Failed to patch Binutils sources ==="
   cd ..
   touch .binutils_patched
fi

echo "=== Decompressing and patching GCC sources ==="
if [ ! -d $GCC_VERSION ] ; then
   tar -xzf ../$GCC_ARCHIVE -C .
   check_result "=== Failed to decompress GCC sources ==="
fi
if [ ! -f .gcc_patched ] ; then
   cd $GCC_VERSION
   patch -p1 < ../../$GCC_PATCH
   check_result "=== Failed to patch GCC sources ==="
   cd ..
   touch .gcc_patched
fi

echo "=== Building Binutils ==="
if [ ! -f .binutils_built ] ; then
   cd $BINUTILS_VERSION
   ./configure --target tms9900 --prefix $PREFIX --disable-build-warnings
   check_result "=== Failed to configure Binutils ==="
   # MAKEINFO=true skips building the info docs, which fail with texinfo >= 5
   make -j$JOBS all MAKEINFO=true
   if [ ! $? == 0 ] ; then
     # Work around a long-standing binutils build ordering bug: on this
     # first pass, ld's Makefile can generate e<emulation>.c (eg
     # eelf32tms9900.c) via genscripts.sh before the ldscripts/
     # directory it writes into has been created, leaving the linker
     # script text empty and producing "'return' with no value" compile
     # errors.  ld/Makefile now exists (it's generated on this first
     # pass), so forcing its ldscripts/stamp target and retrying
     # completes the build cleanly.
     echo "=== Retrying Binutils build after forcing ld/ldscripts ==="
     make -C ld ldscripts/stamp MAKEINFO=true
     check_result "=== Failed to build Binutils ld/ldscripts ==="
     make -j$JOBS all MAKEINFO=true
     check_result "=== Failed to build Binutils ==="
   fi
   make install MAKEINFO=true
   check_result "=== Failed to install Binutils ==="
   cd ..
   touch .binutils_built
fi

echo "=== Building GCC ==="
if [ ! -f .gcc_built ] ; then
   cd $GCC_VERSION
   mkdir build
   cd build
   ../configure --prefix $PREFIX --target=tms9900 --enable-languages=c,c++ $GCC_CONFIGURE_EXTRA
   check_result "=== Failed to configure GCC ==="
   make -j$JOBS all-gcc MAKEINFO=true
   check_result "=== Failed to build GCC ==="
   make -j$JOBS all-target-libgcc MAKEINFO=true
   check_result "=== Failed to build libgcc ==="
   make install-gcc install-target-libgcc MAKEINFO=true
   # Make install has an expected failure:
   #  /bin/bash: line 3: cd: tms9900/libssp: No such file or directory
   # We do not build libssp, so that's OK
   cd ../..
   touch .gcc_built
fi

echo "=== Building libgcc.a ==="
if [ ! -f .libgcc_built ] ; then
   cd $GCC_VERSION
   mkdir build
   cd build
   make -j$JOBS all-target-libgcc MAKEINFO=true
   check_result "=== Failed to build libgcc.a ==="
   make install-target-libgcc MAKEINFO=true
   check_result "=== Failed to build libgcc.a ==="
   cd ../..
   touch .libgcc_built
fi

echo "=== Building elfutils ==="
if [ ! -f .elfutils_built ] ; then
   cd ../elfutils
   make install PREFIX=$PREFIX
   cd ../build
   touch .elfutils_built
fi

echo "=== Installation complete ==="
cd $START

