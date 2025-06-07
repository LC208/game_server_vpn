#!/bin/bash
set -e 

#conan install . -of build/conan -pr:b clang19-cpp20 -pr:h clang19-cpp20 --build=missing
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

ctest --test-dir build -V