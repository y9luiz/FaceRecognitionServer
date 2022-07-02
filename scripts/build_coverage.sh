#!/bin/bash
echo "BUILDING PROJECT AS CODE COVERAGE"
mkdir build_coverage
cd build_coverage
cmake   -D CMAKE_CXX_COMPILER=/usr/bin/clang++  -D CMAKE_C_COMPILER=/usr/bin/clang -D CMAKE_BUILD_TYPE=Coverage ..
cmake --build .