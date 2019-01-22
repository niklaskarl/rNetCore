#!/bin/bash

pushd ./.tests-unix-gcc/test/stringtranscoding/
./stringtranscoding_test
popd

pushd ./.tests-unix-gcc/test/semver/
./semver_test
popd

pushd ./.tests-unix-gcc/test/clrhost/
./clrhost_test
popd
