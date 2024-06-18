#!/bin/sh

SRC_DIR=./
BUILD_DIR=$SRC_DIR/build

# clean bios
make -C $SRC_DIR/src/bios clean

# clean emulator
rm -rf $BUILD_DIR