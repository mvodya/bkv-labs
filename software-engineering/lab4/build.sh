#!/bin/bash
echo "Starting building via g++"
g++ -std=c++11 src/*.cpp src/*.h -lfltk -o "sim"
echo "Build complite! (or not, this script don't know)"
echo "Run: ./sim"
