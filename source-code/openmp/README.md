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
1. `vector_product_gpu.c`: application that computes a vector product
   on the GPU.
1. `vector_product_cpu.c`: application that computes a vector product
   on the CPU.
1. `julia_omp_cpu.c`: application that computes a Julia set on the CPU.
1. `julia_omp_gpu.c`: application that computes a Julia set on the GPU.
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
