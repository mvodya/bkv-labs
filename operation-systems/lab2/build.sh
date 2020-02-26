#!/bin/bash

echo "Building..."

g++ bitmapper.cpp -o bitmapper
g++ randomdd.cpp -o randomdd

chmod +x bitmapper
chmod +x randomdd

echo "OK"