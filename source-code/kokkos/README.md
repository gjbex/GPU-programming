# Kokkos

Kokkos is a framework for heterogenious programming in C++. It supports
several backends such as OpenMP, CUDA, HIP, and SYCL.


## What is it?

1. `hello_world.cpp`: very simple application illustrating a
   `Kokkos::paralle_for`.
1. `vector_product.cpp`: vector multiplication using classes.
1. `vector_product_lambdas.cpp`: vector multiplication using lambdas.
1. `random_numbers.cpp`: illustrates deep copy of views and reduction
   to a non-scalar.
1. `pi.cpp`: Monte Carlo approximation of pi, illustrating the use of
   `Kokkos::Random_X` classes.
1. `matrix_vector.cpp`: matrix-vector multiplication illustrating the
   use of `Kokkos::Fence`.
1. `matrix_matrix.cpp`: matrix-matrix multiplication illustrating the
   use of `Kokkos::MDRangePolicy`.
1. `matrix_matrix_cpu.cpp`: matrix-matrix multiplication illustrating the
   use of `Kokkos::MDRangePolicy`, running on CPU to benchmark speedup.
1. `CMakeLists.txt`: CMake file to build the applications.
