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


# Base all following work on latest patch versions
BINUTILS_PATCH=`latest binutils-*.patch`
MDEF_BINUTILS_PATCH=`latest multidef-binutils-*.patch`
GCC_PATCH=`latest gcc-*.patch`

# Extract tool versions from patch name
BINUTILS_VERSION=`echo $BINUTILS_PATCH | sed "s/-tms9900.*//"`
GCC_VERSION=`     echo $GCC_PATCH      | sed "s/-tms9900.*//"`

# Compose name of source archive
BINUTILS_ARCHIVE="$BINUTILS_VERSION.tar.bz2"
GCC_ARCHIVE="$GCC_VERSION.tar.gz"

# Select download tool
if [ ! -z "`which wget`" ]; then
  WGET=wget
elif [ ! -z "`which curl`" ]; then
  WGET=curl -C
fi

# Check arguments
PREFIX=$1
if [ -z $PREFIX ] ; then
   echo Error: No output directory specified
   echo
   echo Usage:
   echo   install.sh DIRECTORY
   echo The output binaries willl be installed at DIRECTORY/bin
   exit
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
   make all
   check_result "=== Failed to build Binutils ==="
   make install
   check_result "=== Failed to install Binutils ==="
   cd ..
   touch .binutils_built
fi

echo "=== Building GCC ==="
if [ ! -f .gcc_built ] ; then
   cd $GCC_VERSION
   mkdir build
   cd build
   ../configure --prefix $PREFIX --target=tms9900 --enable-languages=c,c++
   check_result "=== Failed to configure GCC ==="
   make all-gcc
   check_result "=== Failed to build GCC ==="
   make install install-target-libgcc
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
   make  all-target-libgcc
   check_result "=== Failed to build libgcc.a ==="
   make install-target-libgcc
   check_result "=== Failed to build libgcc.a ==="
   cd ../..
   touch .libgcc_built
fi

# TODO built elf2ea5 and elf2cart
echo "=== Installation complete ==="
cd $START

