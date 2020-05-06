#!/bin/bash

# WORK IN PROGRESS

# Operation systems
# Lab 8 - Passwd lab / part 2
# by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

re='^[0-9]+$'
for dir in *; do
  if [[ $dir =~ $re ]] && [ -d $dir ] ; then
     cd $dir
     echo -e "${BLUE}Parsing computer ${GREEN}#$dir:${NC} "
     echo -e "${BLUE}Users without password:${NC}"
     echo | awk -vdir=$dir -F: '$2 == "" && $3 > 1000 && $3 < 65000 { print $1 }' passwd | pr -at3
     echo -e "${BLUE}Duplicated user names:${NC}"
     cat passwd | cut -d: -f1 | sed "s/\(\w*\)_\(.\)_\(.\)/\1_\2\3/g" | uniq -d | pr -at3

     #cat passwd | cut -d: -f1 > cpasswd
     #echo "Unknown users:"
     #sort ../passwdcmp cpasswd | uniq -u

     #cat passwd | cut -d: -f1 >> ../passwdcmp

     cd ..
     echo ""
  fi
done  