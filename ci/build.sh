#!/bin/bash

set -e

CFLAGS="-Wall -pedantic"

mkdir -p build

gcc -std=c99 -g args.c example/example.c -o build/cexample
g++ -std=c++11 -x c++ args.c example/example.c -o build/c++example

g++ -std=c++11 -x c++ -g args.c tests/test.cpp -o build/runtests
