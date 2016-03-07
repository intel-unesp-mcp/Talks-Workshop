#!/bin/bash

export KMP_AFFINITY=scatter
#export KMP_AFFINITY=compact

time ./matrix.simd.icc >> t
time ./matrix.simd.icc >> t
time ./matrix.simd.icc >> t
time ./matrix.simd.icc >> t
time ./matrix.simd.icc >> t
time ./matrix.nosimd.icc > t
time ./matrix.nosimd.icc >> t
time ./matrix.nosimd.icc >> t
time ./matrix.nosimd.icc >> t
time ./matrix.nosimd.icc >> t
