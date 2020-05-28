#!/bin/bash
clang++ -mavx -O3 taskB_avx.cpp  && time ./a.out