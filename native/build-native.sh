#!/bin/bash
set -e

mkdir -p build
cd build

cmake -D CMAKE_C_COMPILER=${CC:-gcc} ..
cmake --build .
ctest