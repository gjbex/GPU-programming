cmake_minimum_required(VERSION 3.16)
project(KokkosTutorialTasking)
include(../common.cmake)

add_executable(fib.exe fib.cpp)
target_link_libraries(fib.exe Kokkos::kokkos)
