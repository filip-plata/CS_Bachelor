#!/usr/bin/env bash

ERROR_COUNT=0
TEST_CASE="03"
PORT=20160
CLIENTS_N=7
DATA_FILE="data_${TEST_CASE}.txt"


echo -n "1493448119 c " > real_out.txt
cat "${DATA_FILE}" >> real_out.txt
echo >> real_out.txt

./server 20160 ${DATA_FILE} &
sleep 0.2

for f in $(seq 1 ${CLIENTS_N})
do
    ./client 1493448119 c localhost > client_${f}.txt &
    sleep 0.3
done

sleep 0.5

kill $(jobs -p)
wait $(jobs -p) 2> /dev/null

for f in $(seq 1 ${CLIENTS_N})
do
    FILE_DUP=$((${CLIENTS_N} - ${f} + 1))
    yes real_out.txt | head -n ${FILE_DUP} | xargs cat > /tmp/tmp.txt
    diff /tmp/tmp.txt client_${f}.txt > /dev/null
    df=$?
    if [ ${df} -ne 0 ]
    then
        echo "Client ${f} printed wrong data!" >&2
        ERROR_COUNT=$((${ERROR_COUNT} + 1))
    fi

    rm -f client_${f}.txt
done

rm -f /tmp/tmp.txt real_out.txt

if [ ${ERROR_COUNT} -ne 0 ]
then
    echo "TEST ${TEST_CASE} FAILED!" >&2
else
    echo "TEST ${TEST_CASE} OK!" >&2
fi
