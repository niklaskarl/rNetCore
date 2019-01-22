#!/bin/bash

if [ ! -d ".tests-unix-gcc" ]; then
    mkdir ".tests-unix-gcc"
fi

pushd ".tests-unix-gcc"
cmake -G "Unix Makefiles" ".." -DCMAKE_BUILD_TYPE=Debug
make
popd
