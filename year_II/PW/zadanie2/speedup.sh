#!/usr/bin/env bash

if [[ $# -eq 0 ]]
then
    echo "USAGE: first argument is path to project binary, second input file and the last one is output file path"
    exit 0
fi

BINARY=$1
INPUT=$2
OUTPUT=$3
TIME=0

rm -f ${OUTPUT}

for j in {1..8}
do
    start=$(date +%s.%N)
    ./${BINARY} ${j} ${INPUT} "xd"
    TIME=$(echo "$(date +%s.%N) - $start" | bc)
    echo "ON ${j} cores: ${TIME}" >> ${OUTPUT}
    rm -f "xd"
done

