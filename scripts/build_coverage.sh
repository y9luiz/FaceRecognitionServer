#!/bin/bash
echo "BUILDING PROJECT AS CODE COVERAGE"

mkdir build_coverage
cd build_coverage
cmake -G Ninja -D CMAKE_BUILD_TYPE=Debug -D ENABLE_CODE_COVERAGE=ON -D CMAKE_CXX_COMPILER=/usr/bin/g++  -D CMAKE_C_COMPILER=/usr/bin/gcc ..
cmake --build .
ctest
lcov -c -d . -o coverage.info
lcov -remove coverage.info '/usr/*' '*google*' '*mock*' '*test*' --output-file coverage.info
genhtml coverage.info --output-directory=../report
