# GEMV

Matrix-vector multiplication example, don't do this in production code, use
a BLAS or equivalent.


## What is it?

1. `matrix_vector_cpu.cpp`: matrix-vector multiplication on CPU as a
   performance baseline.
1. `matrix_vector.cpp`: matrix-vector multiplication implemented as
   a one-level parallel loop, using `Kokkos::parallel_reduce`.
1. `matrix_vector_teams.cpp`: matrix-vector multiplication implemented as
   a two-level parallel loop, using `Kokkos::parallel_reduce` twice, once
   with a `Kokkos::TeamPolicy`, once with a Kokkos::TeamThreadRange`.
1. `matrix_vector_teams_layout_right.cpp`: same as `matrix_vector_teams.cpp`,
   but with the matrix stored in layout right which is not the default, but
   more efficient for this case.
1. `CMakeLists.txt`: CMake file to build the applications.

