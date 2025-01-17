Over the past decade or more, most of the software that uses GPGPUs has been
developed in CUDA since only NVIDIA provided such hardware.  However, currently,
at least three vendors provide GPU accelerators, AMD, Intel and NVIDIA.  This
implies that you need a vendor-agnostic programming model to use GPUs.  OpenMP
is such a model.  It has long been used for shared-memory parallelism, but it
has been extended to support offloading to accelerators.  This training will
show you how to use OpenMP to offload computations to GPUs.


## Learning outcomes

When you complete this training, you will

* understand the principles of offloading computations to GPUs using OpenMP;
* know how to transfer data between the host and the device;
* be able to write programs that offload computations to GPUs.


## Schedule

TBD


## Training materials

TBD


## Target audience

This training is for you if you want to develop software that uses GPUs, and
want to software to be portable across GPUs of different vendors.


## Prerequisites

You should be familiar with C/C++ or Fortran, and have some experience with
OpenMP.  Familiarity with a Linux environment is also required.


## Trainer(s)

* Geert Jan Bex ([geertjan.bex@uhasselt.be](mailto:geertjan.bex@uhasselt.be))
