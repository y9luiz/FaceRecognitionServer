#!/bin/bash
echo "BUILDING PROJECT AS RELEASE"
mkdir build_release
cd build_release
cmake -G Ninja -D CMAKE_CXX_COMPILER=/usr/bin/clang++  -D CMAKE_C_COMPILER=/usr/bin/clang -D CMAKE_BUILD_TYPE=Release ..
cmake --build .