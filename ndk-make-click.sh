#!/bin/sh
if [ -z $CLICK_PREFIX ] 
then
    echo "CLICK_PREFIX is not set, exiting..."
    exit 1
fi

CC="arm-linux-androideabi-gcc" \
CXX="arm-linux-androideabi-g++" \
./configure --host=arm-linux-androideabi \
--prefix="$CLICK_PREFIX" \
CXXFLAGS="-fno-rtti -fno-exceptions" \
LDFLAGS="-lgnustl_shared" \
--enable-tools=host --disable-linuxmodule --disable-analysis --disable-test --disable-nsclick --disable-icmp

make -j2
