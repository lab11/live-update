#!/usr/bin/env bash

GCC_SRC_DIR=$1

NEWLIB_VERSION=2.5.0.20170421

SCRIPTPATH=$( cd $(dirname $0) ; pwd -P )
NEWLIB_INCLUDE_PATH=$SCRIPTPATH/../newlib/newlib-$NEWLIB_VERSION/newlib/libc/include

if [[ ! -e $NEWLIB_INCLUDE_PATH ]]; then
  echo "ERROR: Missing NEWLIB_INCLUDE_PATH, expected"
  echo "       $NEWLIB_INCLUDE_PATH"
  echo ""
  echo "Ensure that appropriate newlib version has been built before building libc++"
  exit 1
fi

export CFLAGS_FOR_TARGET="-g -Os -ffunction-sections -fdata-sections -fPIC -msingle-pic-base -mno-pic-data-is-text-relative -mthumb -march=armv8-m.main -isystem $NEWLIB_INCLUDE_PATH"

if gcc --version | grep -q clang; then
  echo "$(tput bold)System gcc is clang. Overriding with gcc-6"
  echo "$(tput sgr0)You may need to brew install gcc@6 if you haven't."
  echo ""
  sleep 2
  export CC=gcc-6
  export CXX=g++-6

  gmp=$($CC -v 2>&1 | tr " " "\n" | grep gmp)
  mpfr=$($CC -v 2>&1 | tr " " "\n" | grep mpfr)
  mpc=$($CC -v 2>&1 | tr " " "\n" | grep mpc)
  isl=$($CC -v 2>&1 | tr " " "\n" | grep isl)
  extra_with="$gmp $mpfr $mpc $isl"
else
  extra_with=""
fi

#export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu/
$GCC_SRC_DIR/contrib/download_prerequisites

# 6.2.0:
$GCC_SRC_DIR/configure \
  --build=x86_64-linux-gnu \
  --host=x86_64-linux-gnu \
  --target=arm-none-eabi \
  --with-arch=armv8-m.main\
  --with-mode=thumb \
  --disable-fpu \
  --with-newlib $extra_with \
  --with-headers=$NEWLIB_INCLUDE_PATH \
  --enable-languages="c" \

make -j12
