#/bin/bash

#icc -O3 -xhost -g -vec-report6 -c autovec2.c -o autovec2.o -restrict
#icc -O3 -xhost -g -vec-report6 -c matrix.c -o matrix.o -restrict
#icc -g -vec-report6 -O3 -xhost autovec2.o matrix.o -o autovec2.icc -restrict

icc -O3 -xhost -g -vec-report6 -c autovec2.c -o autovec2.o -fargument-noalias
icc -O3 -xhost -g -vec-report6 -c matrix.c -o matrix.o -fargument-noalias
icc -g -vec-report6 -O3 -xhost autovec2.o matrix.o -o autovec2.icc -fargument-noalias

#icc -g -O3 -c autovec2.c -o autovec2.o -fopenmp
#icc -g -O3 -c matrix.c -o matrix.o -fopenmp
#icc -g -O3 -xhost autovec2.o matrix.o -o autovec2.icc -fopenmp
