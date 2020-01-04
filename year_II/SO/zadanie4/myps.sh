#!/usr/bin/env bash

cp callnr.h /usr/src/minix/include/minix/

cp callnr.h /usr/include/minix/

cp proto.h /usr/src/minix/servers/pm/

cp myps.c /usr/src/minix/servers/pm/

cp table.c /usr/src/minix/servers/pm/

cp Makefile /usr/src/minix/servers/pm/

cp unistd.h /usr/include/

cp unistd.h /usr/src/include/

cp myps-lib.c /usr/src/lib/libc/misc/

cp Makefile.inc /usr/src/lib/libc/misc/

make -C /usr/src/minix/servers/pm/

make -C /usr/src/minix/servers/pm/ install

make -C /usr/src/lib/libc/

make -C /usr/src/lib/libc/ install

make -C /usr/src/releasetools/ hdboot
