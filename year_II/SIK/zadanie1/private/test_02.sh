#!/usr/bin/env bash

function try_args {
    RUN_L=$1
    
    timeout 1 ./${RUN_L} 2> /dev/null
    RES_CODE="${?}"

    if [ "${RES_CODE}" -ne 1 ]
    then
      echo "${RUN_L} is supposed to fail"
      exit 1
    fi
}

# TEST client and server
RUN_LINES=("client aaaa a localhost 21553" "client be4 a localhost"
           "client 150000000 45 localhost" "client 150000000 b localhos 15000"
           "server 1000000 xd aa" "client 150000 bb localhost")

for RUN_LINE in "${RUN_LINES[@]}"
do
    try_args "${RUN_LINE}"
done


echo "TEST 02 OK!"
