# OpenMP

The OpenMP standard supports GPU offloading, however, this has
to be implemented by the compiler.

For these examples, the `nvc++` compiler included in the [NVIDIA
HPC SDK](https://developer.nvidia.com/hpc-sdk) is used.


## What is it?

1. `devices_gpu.c`: application that queries the number of available
   GPU devices (useful to test succesful build).
1. `hello_world_gpu.c`: application that checks the number of threads
   and teams on the device.
1. `teams_gpu.c`: application that checks the number of teams on the
   device, and the number of threads in a team.
1. `teams_cpu.c`: application that checks the number of teams on the
   device, and the number of threads in a team.
1. `vector_product_gpu.c`: application that computes a vector product
   on the GPU.
1. `vector_product_cpu.c`: application that computes a vector product
   on the CPU.
1. `vector_product_parallel_for_gpu.c`: application that computes a
   vector product on the GPU using a `parallel for` construct.
1. `vector_product_no_distribute_gpu.c`: application that computes a
   vector product on the GPU using a `parallel for` construct, but
   without the `distribute` clause.
1. `vector_product_2_gpu.c`: application that computes a vector product
   on the GPU, initialization and reduction also done on GPU.
1. `vector_product_2_cpu.c`: application that computes a vector product
   on the CPU, initialization and reduction also done in parallel region.
1. `vector_product_3_gpu.c`: application that computes a vector product
   on the GPU, arrays allocated purely on device.
1. `julia_omp_cpu.c`: application that computes a Julia set on the CPU.
1. `julia_omp_gpu.c`: application that computes a Julia set on the GPU.
1. `device_allocation_gpu.c`: application that demonstrates how to
   allocate memory on the GPU.
1. `device_allocation_declare_gpu.c`: application that demonstrates the
   use of the `alloc` map-type modifier.
1. `structured_data_region_gpu.c`: application that demonstrates
   the use of the `target` directive with a structured data region.
1. `unstructured_data_region_gpu.c`: application that demonstrates
   the use of the `target` directive with an unstructured data region.
1. `heat_gpu.c`: application that solves the heat equation on the GPU and
   illustrates how to update data between device and host.
1. `CMakeLists.txt`: CMake file to build the applications.
1. `compiler_issues`: directory that contains example that can not be
   compiled with the NVIDIA HPC SDK compiler.


## How to build?

If necessary, set the appropriate environment variable to use the NVIDIA
ocmpiler, e.g.,
```bash
$ export CC=nvc
```
