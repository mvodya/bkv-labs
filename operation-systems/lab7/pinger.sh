#!/bin/bash

printf "\033[2J\033[0;0H"

input="hosts.txt"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

declare -a common_offset
declare -a common_addr

while IFS=" " read -r name addr
do
  printf "$name  "
  common_offset+=(`expr length $name`)
  common_addr+=($addr)
done < "$input"

printf "\n"

declare -i offset=-4
declare -i i=0

for off in ${common_offset[@]}
do
  offset=$offset+$off+2
  ping -W 1 -c 1 ${common_addr[i]} &>/dev/null
  if [ $? -eq 0 ]
  then
    printf "\033[2;$(($offset))H${GREEN} on${NC}"
  else
    printf "\033[2;$(($offset))H${RED}off${NC}"
  fi
  i=$((i+1))
done

printf "\n"