# Environments

Recipes to build development environments to compile and run the code.
in this repository.


## What is it?

1. `environment.yml`: conda environment file for a Python environment with
   the necessary packages to run the Python scripts in this repository.
1. `nvidia_hpc.py`: hpccm recipe to build an environment with NVIDIA HPC SDK.
1. `oneapi_hpc.py`: hpccm recipe to build an environment with Intel oneAPI
   HPC Toolkit.
1. `oneapi-for-nvidia-gpus-2024.1.0-cuda-12.0-linux.sh`: script to install
   a backend for Intel oneAPI HPC Toolkit to use NVIDIA GPUs.
1. `kokkos.py`: hpccm recipe to build an environment with the NVIDA HPC SDK
   and the Kokkos framework.
