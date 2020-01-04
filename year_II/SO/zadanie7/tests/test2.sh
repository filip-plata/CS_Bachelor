#!/usr/bin/env bash

# Efficiency tests

. common.sh

BASE_FILE_SIZE=50120
TEST_CASES=5

errcho "Efficiecy tests, number: ${TEST_CASES}" 2>> ${LOG}

for i in `seq ${TEST_CASES}`
do
  FILE_S=$((${BASE_FILE_SIZE} * ${i}))
  errcho "Creating file size ${FILE_S}" 2>> ${LOG}
  { time make_file ${FILE_S} out.raw 0 2>&3; } 3>&2 2>> ${LOG}
  errcho "Copying this file" 2>> ${LOG}
  { time cp ./out.raw /usr/src/copy.raw 2>&3; } 3>&2 2>> ${LOG}
done

## Just to be sure, check if random files give the same speed

errcho "Random file speed should be the same" 2>> ${LOG}

FILE_SIZE=$((${BASE_FILE_SIZE} * 1))
{ time dd if=/dev/urandom of=./out.raw count=${FILE_SIZE} bs=${BLOCK_SIZE}  2>&3; } 3>&2 2>> ${LOG}
errcho "Copy random file" 2>> ${LOG}
{ time cp ./out.raw ./copy.raw 2>&3; } 3>&2 2>> ${LOG}

rm ./out.raw
rm ./copy.raw
