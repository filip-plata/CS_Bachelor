#!/usr/bin/env bash

qemu-system-x86_64 -curses -drive file=minix.img -localtime -net user,hostfwd=tcp::15881-:22 -net nic,model=virtio -enable-kvm -m 512M

echo $! > pid.log
