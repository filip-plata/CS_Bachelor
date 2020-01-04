#!/usr/bin/env bash

GRAMMAR=${1}

bnfc -m -haskell ${GRAMMAR}
make
make clean
rm -f *.x *.y *.txt TestGrammar.hs TestGrammar *.bak Makefile
