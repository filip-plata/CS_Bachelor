#!/usr/bin/env bash

######### Common variables

BLOCK_SIZE=4096

#######################################################################
assert ()                 #  If condition false,
{                         #+ exit from script
                          #+ with appropriate error message.
  E_PARAM_ERR=98
  E_ASSERT_FAILED=99


  if [ -z "$2" ]          #  Not enough parameters passed
  then                    #+ to assert() function.
    return $E_PARAM_ERR   #  No damage done.
  fi

  lineno=$2

  if [ ! $1 ]
  then
    echo "Assertion failed:  \"$1\""
    echo "File \"$0\", line $lineno"    # Give name of file and line number.
    exit $E_ASSERT_FAILED
  # else
  #   return
  #   and continue executing the script.
  fi
} # Insert a similar assert() function into a script you need to debug.
#######################################################################

function errcho {
  >&2 echo "$@"
}

function make_file {
  # make_file FILE_SIZE_KB FILE_NAME FILE_OFFSET
  FILE_S=${1}
  FILE_NAME=${2}
  OFF=${3}
  dd if=/dev/zero seek=${OFF} of=./${FILE_NAME} count=${FILE_S} bs=${BLOCK_SIZE}
}

######### Common startup

LOG=${1}
rm -f ${LOG}

if [ "$#" -ne 1 ]
then
  errcho "Usage $0 log.file"
  exit 1
fi
