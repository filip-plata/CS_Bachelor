#!/usr/bin/env bash

rm -f ./minix.img
qemu-img create -f qcow2 -o backing_file=../minix.img ./minix.img
