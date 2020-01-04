#!/usr/bin/env bash

if [[ $# -eq 0 ]]
then
    echo "USAGE: ./test.sh path_to_binary # invoked in as-is folder structure"
    exit 0
fi

BINARY=$1

for input in tests_small/*.in
do
    for j in {1..100}
    do
        OUT="${input}.${j}.out"
        ./${BINARY} ${j} ${input} ${OUT}
        diff -q ${input%in}out ${OUT} > /dev/null
        EXIT=$?
        if [[ ${EXIT} -ne 0 ]]
        then
            echo "ERROR ON TEST ${input} WITH ${j} cores"
            exit 1
        fi
        rm -f ${OUT}
    done
done

for input in tests_medium/*.in
do
    for j in {3..4}
    do
        OUT="${input}.${j}.out"
        ./${BINARY} ${j} ${input} ${OUT}
        diff -q ${input%in}out ${OUT} > /dev/null
        EXIT=$?
        if [[ ${EXIT} -ne 0 ]]
        then
            echo "ERROR ON TEST ${input} WITH ${j} cores"
            exit 1
        fi
        rm -f ${OUT}
    done
done

