#!/usr/bin/env bash

# Efficiency tests

. common.sh

BASE_FILE_SIZE=50120
TEST_CASES=3

function copy_disc {
    # make_interlacing FILE_SIZE_BLOCK FILE_NAME
    FILE_S=${1}
    FILE_NAME=${2}
    dd if=/dev/c0d0 of=./${FILE_NAME} count=${FILE_S} bs=${BLOCK_SIZE}
}

errcho "Efficiecy tests interlacing, number: ${TEST_CASES}" 2>> ${LOG}

for i in `seq ${TEST_CASES}`
do
  FILE_S=$((${BASE_FILE_SIZE} * ${i}))
  errcho "Creating file size ${FILE_S}" 2>> ${LOG}
  { time copy_disc ${FILE_S} out.raw 2>&3; } 3>&2 2>> ${LOG}
  errcho "Copying this file" 2>> ${LOG}
  { time cp ./out.raw ./copy.raw 2>&3; } 3>&2 2>> ${LOG}
done

rm ./out.raw
rm ./copy.raw
