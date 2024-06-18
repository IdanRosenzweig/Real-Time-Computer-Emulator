#!/bin/sh

SRC_DIR=./
BUILD_DIR=$SRC_DIR/build

if ! test -d $BUILD_DIR; then
  mkdir $BUILD_DIR
fi

# build bios
make -C $SRC_DIR/src/bios
cp $SRC_DIR/src/bios/bios.bin $BUILD_DIR/

# build processor's reset vector
nasm -o $BUILD_DIR/reset_vector.bin $SRC_DIR/src/hardware/cpu_i386/reset_vector.asm

# build whole emulator
cmake -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -G Ninja -S ./ -B $BUILD_DIR -DRESET_VECTOR_FILE_PATH="\"$BUILD_DIR/reset_vector.bin\"" -DBIOS_FILE_PATH="\"$BUILD_DIR/bios.bin\""
cmake --build $BUILD_DIR --target emulator -j 4
