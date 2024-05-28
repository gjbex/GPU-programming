# Compiler issues

Unfortunately, the NVIDIA compiler (24.3) can compile the code in this
directory.


## What is it?

1. `julia_omp_target_declare.c`: to call functions in a target region, the
   functions need to be declared with `omp declare target` in the source file.
1. `device_allocation_gpu.c`: example of allocating memory on the GPU directly
   using omp_target_alloc.
1. `CMakeLists.txt`: CMake file to build the application.
