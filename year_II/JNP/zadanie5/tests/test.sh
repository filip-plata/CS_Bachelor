#!/usr/bin/env bash

for file in *.cc
do
    BINARY=${file%.cc}
    g++ -std=c++14 -Wall -Wpedantic -O2 -o ${BINARY} $file
    ./${BINARY}
    rm -rf ${BINARY}
done
