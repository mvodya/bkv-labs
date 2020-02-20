#!/bin/bash

cmd1="wine `dirname $0`"
cmd1+="/macro11.exe $1 -l listing.txt"
cmd2="`dirname $0`"
cmd2+="/retro"

echo "Running macro 11 assembler..."
$cmd1
if [[ $? != 0 ]]; then
  exit $?
fi

echo "Running retroscoper"
$cmd2 > code.txt

echo "Running pdp11 simulator"
pdp11 -do code.txt
