#!/bin/bash

mkdir build
cd build
cmake .. -DLLVM_DIR='/home/maxim/llvm-install/lib/cmake/llvm'
make

cd ..
cp -f build/src/calc tools/
