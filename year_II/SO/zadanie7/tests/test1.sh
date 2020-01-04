#!/usr/bin/env bash

# Corectness tests

#######################
# Create large files  #
#######################

. common.sh

##############################

FILE_SIZE=10240

##### TEST 1
# File consisting only of 0, assert size is ok

errcho "Creating large file with zeroes..." 2>> ${LOG}

make_file ${FILE_SIZE} out.raw 0 # in block of 4kb

file_size=`wc -c < "./out.raw"`
file_size_kb=$((${file_size} / ${BLOCK_SIZE}))
assert "${FILE_SIZE} -eq ${file_size_kb}" ${LINENO} 2>> ${LOG}


#### TEST 2
# File with many zeros inside and random blocks
# at the start and the end

errcho "File with zero blocks inside..." 2>> ${LOG}

dd if=/dev/urandom of=./out.raw count=8 bs=4096
make_file ${FILE_SIZE} out.raw 8 # in blocks of 4kb
dd seek=$((${FILE_SIZE} + 8)) if=/dev/urandom of=./out.raw count=8 bs=4096

file_size=`wc -c < "./out.raw"`
file_size_kb=$((${file_size} / ${BLOCK_SIZE}))
assert "$((${FILE_SIZE} + 16)) -eq ${file_size_kb}" ${LINENO} 2>> ${LOG}
cp ./out.raw ./copy.raw

# Check whether copying works correctly
cmp -s ./out.raw ./copy.raw || assert "0 -eq 1" ${LINENO} 2>> ${LOG}

rm ./out.raw
rm ./copy.raw
