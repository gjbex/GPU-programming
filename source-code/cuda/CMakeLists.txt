cmake_minimum_required(VERSION 3.10)

project(cuda LANGUAGES CUDA)

set(CMAKE_CUDA_STANDARD 14)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_executable(saxpy.exe saxpy.cu)
set_target_properties(saxpy.exe PROPERTIES CUDA_ARCHITECTURES "80")
