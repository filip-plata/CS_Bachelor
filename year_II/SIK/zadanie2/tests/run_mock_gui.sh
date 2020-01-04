#!/usr/bin/env bash
# run_mock_gui.sh

GUI_PID=""

start_gui() {
    ./mock-gui &
    echo ${!}
}

stop_gui() {
    kill ${GUI_PID}
}
