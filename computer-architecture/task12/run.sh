#!/bin/bash
nasm -felf64 program.asm
gcc -no-pie program.c program.o -o rtc
if [ $? -eq 0 ]
 then
  sudo ./rtc
fi
