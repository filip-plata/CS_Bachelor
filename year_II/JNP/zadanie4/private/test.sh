#!/usr/bin/env bash

for file in *.cc
do
    BINARY=${file%.cc}
    clang++ -std=c++14 -Wall -Wpedantic -O2 -lstdc++ -o ${BINARY} $file
    ./${BINARY}
    rm -rf ${BINARY}
done
