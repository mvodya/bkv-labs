#!/bin/bash
nasm -felf64 task8.asm
gcc task8.c task8.o -o task8