#!/usr/bin/env bash

# Efficiency tests

. common.sh

BASE_FILE_SIZE=50120
TEST_CASES=5

function make_interlacing {
    # make_interlacing FILE_SIZE_BLOCK FILE_NAME
    FILE_S=${1}
    FILE_NAME=${2}
    DEVICE=""
    for f in $(seq 1 ${FILE_S})
    do
        if [ $((${f} % 2)) -eq 1 ]
        then
            DEVICE="/dev/zero"
        else
            DEVICE="/dev/urandom"
        fi
        dd if=${DEVICE} seek=${f} of=./${FILE_NAME} count=1 bs=${BLOCK_SIZE}
    done
}

errcho "Efficiecy tests interlacing, number: ${TEST_CASES}" 2>> ${LOG}

for i in `seq ${TEST_CASES}`
do
  FILE_S=$((${BASE_FILE_SIZE} * ${i}))
  errcho "Creating file size ${FILE_S}" 2>> ${LOG}
  { time make_interlacing ${FILE_S} out.raw 2>&3; } 3>&2 2>> ${LOG}
  errcho "Copying this file" 2>> ${LOG}
  { time cp ./out.raw ./copy.raw 2>&3; } 3>&2 2>> ${LOG}
done

rm ./out.raw
rm ./copy.raw
