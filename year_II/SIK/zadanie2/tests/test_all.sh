#!/usr/bin/env bash
# test_all.sh

. assert.sh

./test_command_line_args.sh
echo "test_command_line_args OK"

./test_client_sends_ok.sh
echo "test_client_sends_ok OK"

./siktacka-tests
echo "siktacka-tests OK"
