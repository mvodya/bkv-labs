#!/bin/bash
nasm -felf64 alarm.asm
gcc -no-pie task10.c alarm.o -o alarm
if [ $? -eq 0 ]
 then
  sudo ./alarm
fi