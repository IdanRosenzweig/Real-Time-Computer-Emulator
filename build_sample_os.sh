#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "give the name of the sample os to build (small_os/tiny_os)"
fi


SRC_DIR=./
BUILD_DIR=$SRC_DIR/build

if ! test -d $BUILD_DIR; then
  mkdir $BUILD_DIR
fi

# build the os
if [[ "$1" == "small_os" ]]; then
  make -C $SRC_DIR/samples/small_os
  mv $SRC_DIR/samples/small_os/kernel.img $BUILD_DIR/os_image
  exit 0
fi

if [[ "$1" == "tiny_os" ]]; then
  nasm -f bin -o $SRC_DIR/samples/tiny_os/tiny_os.bin $SRC_DIR/samples/tiny_os/tiny_os.asm
  dd status=noxfer conv=notrunc if=$SRC_DIR/samples/tiny_os/tiny_os.bin of=$BUILD_DIR/os_image
  exit 0
fi

echo "invalid chioce of os"
exit 1
