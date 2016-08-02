Memory Access Optimization

Padding

icc padd.c -o padd
./padd

different strides on the same code

icc vec.c -o vec -O3

auto-vectozation

```O3
icc autoO3.c -o autoO3 -O0

time ./autoO3

real    0m8.428s
user    0m8.422s
sys     0m0.001s
```

```
icc autoO3.c -o autoO3 -O3 -vec-report6

time ./autoO3

real    0m0.003s
user    0m0.001s
sys     0m0.001s
```

```
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
```

```
icc autovec.c -o autovec -vec-report=6
cat autovec.optrpt

icc autovec.c -o autovec -vec-report=6 -O3
cat autovec.optrpt

icc autovec.c -o autovec -vec-report=6 -g -O3
cat autovec.optrpt

icc autovec.c -o autovec -vec-report=6 -g -O3 -xhost
cat autovec.optrpt
```

loop that do not auto vectorize

```
icc novec.c -o novec -vec-report=6 -g -O3
cat novec.optrpt
```

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

-axfeature

Guided Vectorization

Loop not vectorize

```
icc -vec-report6 -c -O3 -xhost autovec2.c -o autovec2.o
icc -vec-report6 -c -O3 -xhost matrix.c -o matrix.o
icc -vec-report6 -O3 -xhost autovec2.o matrix.o -o autovec2.icc
```

```
LOOP BEGIN at matrix.c(22,3)
   remark #15344: loop was not vectorized: vector dependence prevents vectorization
   remark #15346: vector dependence: assumed OUTPUT dependence between c line 27 and c line 27
   remark #15346: vector dependence: assumed OUTPUT dependence between c line 27 and c line 27

   LOOP BEGIN at matrix.c(23,5)
      remark #15344: loop was not vectorized: vector dependence prevents vectorization
      remark #15346: vector dependence: assumed OUTPUT dependence between c line 27 and c line 27
      remark #15346: vector dependence: assumed OUTPUT dependence between c line 27 and c line 27

      LOOP BEGIN at matrix.c(24,7)
         remark #15344: loop was not vectorized: vector dependence prevents vectorization
         remark #15346: vector dependence: assumed FLOW dependence between c line 27 and c line 27
         remark #15346: vector dependence: assumed ANTI dependence between c line 27 and c line 27
      LOOP END

      LOOP BEGIN at matrix.c(24,7)
      <Remainder>
      LOOP END
   LOOP END
LOOP END
```

after pragma simd

```
LOOP BEGIN at matrix.c(25,7)
         remark #15389: vectorization support: reference a has unaligned access   [ matrix.c(27,4) ]
         remark #15389: vectorization support: reference b has unaligned access   [ matrix.c(27,4) ]
         remark #15381: vectorization support: unaligned access used inside loop body
         remark #15415: vectorization support: gather was generated for the variable b:  indirect access, 64bit indexed   [ matrix.c(27,34) ]
         remark #15305: vectorization support: vector length 4
         remark #15399: vectorization support: unroll factor set to 4
         remark #15309: vectorization support: normalized vectorization overhead 0.231
         remark #15301: SIMD LOOP WAS VECTORIZED
         remark #15442: entire loop may be executed in remainder
         remark #15450: unmasked unaligned unit stride loads: 2
         remark #15458: masked indexed (or gather) loads: 1
         remark #15475: --- begin vector loop cost summary ---
         remark #15476: scalar loop cost: 27
         remark #15477: vector loop cost: 6.750
         remark #15478: estimated potential speedup: 3.530
         remark #15488: --- end vector loop cost summary ---
      LOOP END
```


numactl
```
time numactl --cpubind=1 --membind=0 ./matrix.icc
time numactl --cpubind=1 --membind=1 ./matrix.icc
```

References

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and

https://software.intel.com/en-us/articles/data-alignment-to-assist-vectorization

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and

http://www.cs.uu.nl/docs/vakken/mov/
