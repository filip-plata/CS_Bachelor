#!/usr/bin/env bash

PREPARE="pkgin -y in nasm; pkgin -y in hexedit; nasm bootloader.asm -o bootloader"

COPY_ORIG="dd bs=512 count=1 if=/dev/c0d0 of=./orig_bootloader"

COPY_OLD_BOOT="dd seek=512 count=512 bs=1 if=./orig_bootloader of=/dev/c0d0"
COPY_NEW_BOOT="dd count=446 bs=1 if=./bootloader of=/dev/c0d0"

COPY_BOOT="${COPY_OLD_BOOT}; ${COPY_NEW_BOOT};"

SCRIPT="${PREPARE}; ${COPY_ORIG}"

sshpass -p 'root' scp ./bootloader.asm minix:/root

sshpass -p 'root' ssh minix "${SCRIPT}"

sshpass -p 'root' ssh minix "${COPY_OLD_BOOT}"

sshpass -p 'root' ssh minix "${COPY_NEW_BOOT}"

sshpass -p 'root' scp ./.profile minix:/root/

sshpass -p 'root' ssh minix "chmod u+x /root/.profile"
