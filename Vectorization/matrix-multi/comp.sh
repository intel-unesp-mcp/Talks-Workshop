#/bin/bash

icc -g -c -O3 -xhost vec.c -o autovec2.o -vec-report6
icc -g -c -O3 -xhost matrix.c -o matrix.o -vec-report6
icc -g -O3 -xhost autovec2.o matrix.o -o autovec2.icc -vec-report6
