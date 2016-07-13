padding

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

References

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and

https://software.intel.com/en-us/articles/data-alignment-to-assist-vectorization

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and
