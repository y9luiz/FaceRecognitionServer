#!/bin/bash
if [ -d build_debug ];
then
    cd build_debug && ctest .
elif [ -d build_release ];
then
    cd build_release && ctest .
elif [ -d build_coverage ];
then
    cd build_coverage && ctest .
fi
