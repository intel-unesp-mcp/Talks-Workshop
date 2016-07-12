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

References

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and

https://software.intel.com/en-us/articles/data-alignment-to-assist-vectorization

https://software.intel.com/en-us/articles/memory-management-for-optimal-performance-on-intel-xeon-phi-coprocessor-alignment-and
