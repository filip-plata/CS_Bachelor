#!/usr/bin/env bash

for file in ../*.cc
do
    g++ -std=c++14 -Wall -Wpedantic -O2 -c ${file}
done


for file in *.cc
do
    BINARY=${file%.cc}
    g++ -std=c++14 -Wall -Wpedantic -O2 *.o -o ${BINARY} ${file}
    ./${BINARY}
    rm -rf ${BINARY}
done

rm -f *.o

