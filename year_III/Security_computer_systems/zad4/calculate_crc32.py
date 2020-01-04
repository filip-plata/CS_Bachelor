#!/usr/bin/env python

import binascii
import sys

def CRC32_from_file(filename):
    buf = open(filename,'rb').read()
    buf = (binascii.crc32(buf) & 0xFFFFFFFF)
    return "%08X" % buf

if __name__ == "__main__":
    print(CRC32_from_file(sys.argv[1]))
