#!/usr/bin/env bash

for file in *.in
do
    OUT=${file}.curout
    ./Zadanie3 < ${file} > ${OUT}
    diff ${OUT} ${file%in}out > /dev/null 2>&1
    if [ $? != 0 ]
    then
        echo "ERROR ON TEST: ${file}"
        exit 1
    else
        rm ${OUT}
    fi
done
