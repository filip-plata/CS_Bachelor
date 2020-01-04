#!/usr/bin/env bash

MAX_PID=$(cat /proc/sys/kernel/pid_max)

for i in $(seq 1 ${MAX_PID});
do
    >&2 echo "Trying PID ${i}"
    export MYPID=${i}
    rm -f ~/.rnd
    LD_PRELOAD=mygetpid.so LD_LIBRARY_PATH=./openssl-1.1.0g/ ./openssl-1.1.0g/apps/openssl genrsa -out /tmp/key.pem
    openssl rsa -in /tmp/key.pem -pubout > /tmp/test.pub
    if diff -q /tmp/test.pub public.pem
    then
	mv /tmp/key.pem ./solved.pem
	rm /tmp/test.pub
	exit 0
    fi
    
done

