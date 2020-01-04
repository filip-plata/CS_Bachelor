#!/usr/bin/env bash

DEFAULT_TIMEOUT=1

. assert.sh

## Invalid argument
timeout ${DEFAULT_TIMEOUT} ./siktacka-server -W aa -H 12 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -W -14 -H 12 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -L 8 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -t -1 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -S 12 -p 1025 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -H 12 -s -1 -p 1025 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -H -12 -p 1025 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-server -H -12 -p 1025 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "player 1" 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "player1" localhost:80000 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "player1" localhost:8008 localhos 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "p" 127.0.0.1:80000 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "p" googlesdsd.com:8008 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "p" googlesdsd.com:8008:4 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "p" 127.0.0.1:8008a 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" 127.0.0.1:8008 localhost 7 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" 127.0.0.1:8008 localost 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" 127.0.0.1:8008 localhost:65536 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" 127.0.0.1:65534 localhost:-100 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "invalid player" 127.0.0.1:6555 localhost:10000 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" 127.0.0.1:65534 localhost:-100 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" localhost [2001:db8:a0b:12f0::1]:65536 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "invalidplayernametoolongtoactuallyprintitonthescreentoolonglonglong" localhost 2> /dev/null
assert "${?} -eq 1" ${LINENO}

timeout ${DEFAULT_TIMEOUT} ./siktacka-client "" localhost 2001:db8:ua0b:12f0::1:65536 2> /dev/null
assert "${?} -eq 1" ${LINENO}

echo "Command line ok"
