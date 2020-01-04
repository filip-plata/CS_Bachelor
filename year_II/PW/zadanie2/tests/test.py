#!/usr/bin/env python3
import sys
import os
import functools
import subprocess
import collections
import filecmp
from math import isclose

Test_spec_mul = collections.namedtuple("Test_spec",
                                       ["test_dir", "core_numbers"])

TESTS_SMALL= Test_spec_mul(test_dir="tests_small",
                           core_numbers=range(1, 100))
TESTS_MEDIUM = Test_spec_mul(test_dir="tests_medium",
                             core_numbers=range(4, 6))
test_dirs = [TESTS_SMALL, TESTS_MEDIUM]


OUTPUT_PRECISION = 0.0001

def parse_line(line):
    line.rstrip()
    return [t(s) for t,s in zip((int, float), line.split())]

def compare_out(user_out, correct_out, precision=OUTPUT_PRECISION):
    result = True
    with open(user_out) as f1, open(correct_out) as f2:
        us_o = parse_line(f1.readline())
        c_o = parse_line(f2.readline())
        result = result and (us_o[0] == c_o[0]) and isclose(us_o[1], c_o[1],rel_tol=precision)
    return result


def test_case(binary, cores, input, out, correct_out):
    subprocess.call([binary, str(cores), input, out])
    return compare_out(out, correct_out)


def test_directory(test_spec, binary):
    for root, dirs, files in os.walk(test_spec.test_dir):
        for file in files:
            if file.endswith('.in'):
                for cores in test_spec.core_numbers:
                    input = os.path.join(root, file)
                    out = os.path.join(root, "{}.{}.out".format(file, cores))
                    r_out = os.path.join(root, file[:-2] + "out")
                    ok = test_case(binary, cores, input, out, r_out)
                    
                    if not ok:
                        err_msg = "ERROR ON {} WITH {} CORES".format(out, cores)
                        print(err_msg, file=sys.stderr)
                        sys.exit(1)
                    
                    os.remove(out)


if __name__ == "__main__":
    BINARY = ""
    try:
        BINARY = str(sys.argv[1])
    except IndexError:
        print("USAGE: python3 test.py path_to_binary", file=sys.stderr)

    list(map(functools.partial(test_directory, binary=BINARY), test_dirs))

