'''Recipe to build and run software using Kokkos.

Usage:
    $ hpccm  --recipe kokkos.py  --format docker
    $ hpccm  --recipe kokkos.py  --format singularity
'''

# Choose a base image
Stage0.baseimage('nvcr.io/nvidia/nvhpc:24.3-devel-cuda_multi-ubuntu22.04')
 
# Install build tools
Stage0 += cmake(eula=True)

# Install editor and other tools
# Stage0 += gnu()

# Install kokkos
Stage0 += kokkos(
        cmake_opts=['-DKokkos_ENABLE_OPENMP=ON'],
        version='4.3.01',
        )

# Install debugging tools
# Stage0 += apt_get(ospackages=['valgrind', 'strace', 'cppcheck', 'gdb', 'gdbserver'])

# Install benchmark tools
# Stage0 += apt_get(ospackages=['hyperfine', ])

# add run script, i.e., start bash
Stage0 += runscript(commands=['/bin/bash'])
