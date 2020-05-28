#!/bin/bash

echo -e "Just gaussian invertable matrix 1024x1024 solving: "
g++ taskA_v3.cpp -DMATRIX_SIZE=1024 && time ./a.out > /dev/null

echo -e "\n\nWith SSE gaussian invertable matrix 1024x1024 solving: "
g++ -msse taskB_sse.cpp -DMATRIX_SIZE=1024 && time ./a.out > /dev/null

echo -e "\n\nWith AVX gaussian invertable matrix 1024x1024 solving: "
clang++ -mavx -O3 -DMATRIX_SIZE=1024 taskB_avx.cpp  && time ./a.out > /dev/null