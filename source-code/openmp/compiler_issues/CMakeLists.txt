cmake_minimum_required(VERSION 3.20)

project(compiler_issues LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(OpenMP REQUIRED)

add_executable(julia_omp_target_declare.exe
    julia_omp_target_declare.c)
target_compile_options(julia_omp_target_declare.exe
    PRIVATE -fopenmp)
target_link_options(julia_omp_target_declare.exe
    PRIVATE -fopenmp)
target_link_libraries(julia_omp_target_declare.exe
    PRIVATE OpenMP::OpenMP_C)
