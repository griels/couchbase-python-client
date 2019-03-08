#!/usr/bin/env bash
export CMAKE_BUILD_DIR=cmake-build-debug
rm -rf build && python setup.py build_ext --inplace --include-dirs ${CMAKE_BUILD_DIR}/install/include/ --library-dirs ${CMAKE_BUILD_DIR}/install/lib/Debug install &> build.txt &
