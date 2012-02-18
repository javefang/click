#!/bin/sh
CC="arm-linux-androideabi-gcc" \
CXX="arm-linux-androideabi-g++" \
./configure --host=arm-linux-androideabi \
--prefix=/data/click \
CXXFLAGS="-fno-rtti -fno-exceptions -g" \
LDFLAGS="-lgnustl_shared" \
--disable-linuxmodule --enable-tools=host
