# Kokkos

Kokkos is a framework for heterogenious programming in C++. It supports
several backends such as OpenMP, CUDA, HIP, and SYCL.


## What is it?

1. `device.cpp`: simple application displaying the default execution
   space.
1. `exec_space_info.cpp`: simple application displaying the default
   execution space and default host execution space configuration.
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
1. `host_to_host_deepcopy.cpp`: illustration that deep copy of a mirror
   view is a no-op when the source and target are both on the host.
1. `parallel_scan.cpp`: illustration of parallel scan.
1. `parallel_filter.cpp`: illustration of how to combine `Kokkos::parallel_for`
   and `Kokkos::parallel_scan` to filter elements of a view.
1. `min_reductor.cpp`: illustration of using a `Kokkos::Min` reductor.
1. `atomics.cpp`: illustration of using atomic operations to avoid race conditions.
1. `reduce_vs_atomics.cpp`: illustration of using atomics to implement a reduction,
   showing that it is slower than using a `Kokkos::parallel_reduce`.
1. `teams.cpp`: illustration of using `Kokkos::TeamPolicy` to create
   teams of threads.
1. `single.cpp`: illustration of using `Kokkos::single` to execute
   code on a single thread.
1. `gemv`: matrix-vector multiplication example, show the use of
   teams.
1. `gemm`: matrix-matrix multiplication example, show the use of
   `MDRangePolicy`.
1. `random_walks`: simple application to illustrate the use of
  `Kokkos::Random_X` classes as well as memory traits.
1. `cuda_required`: examples that won't compile without `Kokkos_ENABLE_CUDA`.
1. `tasking`: simple example of using tasking.
1. `subviews.cpp`: illustration of using subviews.
1. `dualview`: illustration of using `Kokkos::DualView`.
1. `CMakeLists.txt`: CMake file to build the applications.
