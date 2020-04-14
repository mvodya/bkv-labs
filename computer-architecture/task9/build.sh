#!/bin/bash
nasm -felf64 sqe.asm
gcc -no-pie prog.c sqe.o -o squ
if [ $? -eq 0 ]
 then
  ./squ
fi