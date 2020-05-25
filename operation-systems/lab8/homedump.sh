#!/bin/bash

# Operation systems
# Lab 8 - Passwd lab / part 1
# by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'

ip_arr=()
for i in {101..114}
do
  ip="10.40.14."$i
  ping -W 1 -c 1 $ip &>/dev/null
  if [ $? -eq 0 ]
  then
    ip_arr+=($ip)
  fi
  printf "\rScanned: ${BLUE}"$ip${NC}
done
printf "\r                    \r"
echo ${ip_arr[*]}

echo "Now adding all keys on scanned hosts"

for ip in ${ip_arr[*]}
do
  ssh-copy-id -o "StrictHostKeyChecking no" $ip -f
  if [ $? -eq 1 ]
  then
    delete=($ip)
    ip_arr=("${ip_arr[*]/$delete}")
  fi
done

echo -e ${GREEN}"All keys added!"${NC}  
echo "Copying /etc/passwd and get list from servers..."

for ip in ${ip_arr[*]} 
do
  oct=$(echo $ip | sed 's/10.40.14.//')
  mkdir -p $oct
  cd $oct
  scp $ip:/etc/passwd . &>/dev/null
  ssh -o "StrictHostKeyChecking no" $ip "ls -al /home/" > homels.txt
  printf "\rWorking with ${BLUE}"$ip${NC}
  cd ..
done
printf "\r                         \r"
