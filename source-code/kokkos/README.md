# Kokkos

Kokkos is a framework for heterogenious programming in C++. It supports
several backends such as OpenMP, CUDA, HIP, and SYCL.


## What is it?

1. `device.cpp`: simple application displaying the default execution
   space.
1. `hello_world.cpp`: very simple application illustrating a
   `Kokkos::paralle_for` using a functor.
1. `hello_world_lambdas.cpp`: very simple application illustrating a
   `Kokkos::paralle_for` using a lambda function.
1. `vector_product.cpp`: vector multiplication using a functor.
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
1. `host_to_host_deepcopy.cpp`: illustration that deep copy of a mirror
   view is a no-op when the source and target are both on the host.
1. `parallel_scan.cpp`: illustration of parallel scan.
1. `parallel_filter.cpp`: illustration of how to combine `Kokkos::parallel_for`
   and `Kokkos::parallel_scan` to filter elements of a view.
1. `min_reductor.cpp`: illustration of using a `Kokkos::Min` reductor.
1. `CMakeLists.txt`: CMake file to build the applications.
