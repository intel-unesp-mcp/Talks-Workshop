Memory Access Optimization

Padding

icc padd.c -o padd
./padd

dynamic allocation - aligned
```
_mm_malloc
_mm_free
```

static allocation - aligned
```
__attribute__((align(n)) - new
__declspec(align(n)) - old
```

auto-vectozation

O3
icc autoO3.c -o autoO3 -O0

time ./autoO3

real    0m8.428s
user    0m8.422s
sys     0m0.001s


icc autoO3.c -o autoO3 -O3 -vec-report6

time ./autoO3

real    0m0.003s
user    0m0.001s
sys     0m0.001s


cat autoO3.optrpt

Begin optimization report for: main()

    Report from: Vector optimizations [vec]


LOOP BEGIN at autoO3.c(10,3)
   remark #15344: loop was not vectorized: vector dependence prevents vectorization
   remark #15346: vector dependence: assumed OUTPUT dependence between A line 14 and A line 14
   remark #15346: vector dependence: assumed OUTPUT dependence between A line 14 and A line 14

   LOOP BEGIN at autoO3.c(13,5)
      remark #15414: loop was not vectorized: nothing to vectorize since loop body became empty after optimizations
   LOOP END
LOOP END


icc autovec.c -o autovec -vec-report=6
cat autovec.optrpt

icc autovec.c -o autovec -vec-report=6 -O3
cat autovec.optrpt

icc autovec.c -o autovec -vec-report=6 -g -O3
cat autovec.optrpt

icc autovec.c -o autovec -vec-report=6 -g -O3 -xhost
cat autovec.optrpt

icc novec.c -o novec -vec-report=6 -g -O3
cat novec.optrpt

-axfeature

Guided Vectorization

Loop not vectorizes

icc -vec-report6 -c -O3 -xhost autovec2.c -o autovec2.o
icc -vec-report6 -c -O3 -xhost matrix.c -o matrix.o
icc -vec-report6 -O3 -xhost autovec2.o matrix.o -o autovec2.icc




References

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and

https://software.intel.com/en-us/articles/data-alignment-to-assist-vectorization

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and
