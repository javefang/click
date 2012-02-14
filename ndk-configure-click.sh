#!/bin/sh
SYSROOT="$MY_NDK/sysroot" \
CC="arm-linux-androideabi-gcc" \
CXX="arm-linux-androideabi-g++" \
./configure --host=arm-linux-androideabi \
--prefix=/data/click \
CXXFLAGS="-fno-rtti -fno-exceptions" \
--disable-linuxmodule --enable-tools=host
