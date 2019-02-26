#!/usr/bin/env bash
rm -rf build && python setup.py build_ext --inplace --include-dirs cmake-build-debug/install/include/ --library-dirs cmake-build-debug/install/lib/Debug install &> build.txt &
