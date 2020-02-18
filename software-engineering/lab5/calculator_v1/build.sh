# Software engineering
# Lab 5

if [ ! -d "tmp" ]; then
  mkdir "tmp"
fi

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

if ! [ -x "$(command -v g++)" ]; then
  echo "${red}Error:${reset} g++ is not installed."
  exit 1
fi

echo "Building calc-builder™"

if ! g++ -lstdc++fs -std=c++17 calc-builder/builder.cpp -o tmp/builder -lstdc++fs; then
  echo "${red}Error:${reset} calc-builder™ build failed"
  exit 1
fi

echo "Run calc-builder™"

./tmp/builder

rm tmp -r