# Standard Template Library

C++'s Standard Template Library (STL) algorithms can be parallized using
OpenMP or TBB, but also on GPUs.


## What is it?

1. `julia_set_seq.cpp`: sequential implementation of a Julia set
   computation.
1. `julia_set_par.cpp`: parallel implementation of a Julia set
   computation using `std::execution::par`.
1. `julia_set_par_unseq.cpp`: parallel implementation of a Julia set
   computation using `std::execution::par_unseq`.
1. `sort_data.cpp`: example of using `std::execution::par` to sort data.
1. `CMakeLists.txt`: CMake file to build the applications.
