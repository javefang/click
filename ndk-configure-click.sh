#!/bin/sh
SYSROOT="$MY_NDK/sysroot"
LIBGCC=`arm-linux-androideabi-gcc -print-libgcc-file-name`
./configure --host=arm-linux-androideabi \
--prefix=/data/click \
CPPFLAGS="-I$SYSROOT/usr/include" \
CXXFLAGS="-fno-rtti -fno-exceptions" \
LDFLAGS="-Wl,-rpath-link=$SYSROOT/usr/lib,-dynamic-linker=/system/bin/linker" \
LIBS="-lm -lstdc++ -lc $LIBGCC" \
--disable-linuxmodule --enable-tools=host
