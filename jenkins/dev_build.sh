#!/usr/bin/env bash
rm -rf build && python setup.py build_ext --inplace --include-dirs cmake-build-release/install/include/ --library-dirs cmake-build-release/install/lib/Debug install &> build.txt &
