#!/usr/bin/env bash
# test_all.sh

. assert.sh

./test_command_line_args.sh
echo "test_command_line_args OK"

./siktacka-tests
echo "siktacka-tests OK"
