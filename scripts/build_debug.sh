#!/bin/bash
echo "BUILDING PROJECT AS DEBUG"
mkdir build_debug
cd build_debug
cmake   -D CMAKE_CXX_COMPILER=/usr/bin/clang++  -D CMAKE_C_COMPILER=/usr/bin/clang -D CMAKE_BUILD_TYPE=Debug ..
cmake --build .