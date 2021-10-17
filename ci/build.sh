#!/bin/bash

set -e

CFLAGS="-Wall -Wextra -pedantic -Wno-missing-field-initializers"

mkdir -p build

# Should build clean in both C99 and C++11
gcc -std=c99 $CFLAGS -g args.c example/example.c -o build/cexample
g++ -std=c++11 $CFLAGS -x c++ args.c example/example.c -o build/c++example
g++ -std=c++11 $CFLAGS -x c++ -g args.c tests/test.cpp -o build/runtests
