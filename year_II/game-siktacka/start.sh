#!/usr/bin/env bash

GUI_PORT="${1}"
PLAYER_NAME="${2}"

./gui3/gui3 ${GUI_PORT} > /dev/null 2>&1 &
./build/siktacka-client ${PLAYER_NAME} ec2-52-29-24-148.eu-central-1.compute.amazonaws.com localhost:${GUI_PORT} > /dev/null 2>&1
