# GEMM

Matrix multiplication example, don't do this in production code, use
a BLAS or equivalent.


## What is it?

1. `matrix_matrix.cpp`: matrix-matrix multiplication illustrating the
   use of `Kokkos::MDRangePolicy`.
1. `matrix_matrix_cpu.cpp`: matrix-matrix multiplication illustrating the
   use of `Kokkos::MDRangePolicy`, running on CPU to benchmark speedup.
1. `CMakeLists.txt`: CMake file to build the applications.
