GPUs are increasingly used in scientific computing.  Unfortunately,
multiple vendors have their own programming model for GPUs, e.g, CUDA
for NVIDIA GPUs and HIP for AMD GPUs.  This makes it difficult to
write portable code that can run on different GPUs.  This training
will focus on programming paradigms that can be used to write portable
code for GPUs.

Currently, the training is in preparation and will be extended in the
near future.  For now, the training covers OpenMP offloading and
using the Kokkos library.


## Learning outcomes

When you complete this training you will be able to

  * use OpenMP offloading to write portable code for GPUs in C, C++ or Fortran;
  * use the Kokkos library to write portable code for GPUs in C++;
  * understand pitfalls and best practices for writing portable code for GPUs.


## Schedule

Total duration: 6 hours, split over two sessions.


### Session one: OpenMP offloading

Total duration: 2 hours

  | Subject                                     | Duration |
  |---------------------------------------------|----------|
  | introduction to OpenMP and motivation       |  5 min.  |
  | OpenMP worksharing on device target         | 35 min.  |
  | hands-on session                            | 20 min.  |
  | coffee break                                |  5 min.  |
  | data movement between host and device       | 35 min.  |
  | hands-on session                            | 20 min.  |
  | wrap up                                     |  5 min.  |


### Session two: Kokkos

Total duration: 4 hours

  | Subject                             | Duration |
  |-------------------------------------|----------|
  | introduction to Kokkos              |  5 min.  |
  | Kokkos programming model            | 10 min.  |
  | Recap of C++ features               | 15 min.  |
  | Kokkos execution spaces & dispatch  | 40 min.  |
  | How to build a Kokkos application   |  5 min.  |
  | hands-on session                    | 20 min.  |
  | coffee break                        | 10 min.  |
  | Kokkos memory spaces & views        | 60 min.  |
  | hands-on session                    | 20 min.  |
  | error handling                      | 10 min.  |
  | multi-level parallellization: teams | 40 min.  |
  | additional topics                   | 10 min.  |
  | wrap up                             |  5 min.  |


## Training materials

Slides are available in the
 [GitHub repository](https://github.com/gjbex/GPU-programming).
as well as example code and hands-on material.


## Target audience

This training is for you if you need to develop portable code for GPUs.


## Prerequisites

You will need experience programming in C, C++ or Fortran for the OpenMP
offloading part of the training.  Also note that this part of the training
assumes you are already familiar with OpenMP.

For the Kokkos part of the training you will need experience programming
in C++.  Basic experience with CMake will help.


## Trainer(s)

  * Geert Jan Bex ([geertjan.bex@uhasselt.be](mailto:geertjan.bex@uhasselt.be))
