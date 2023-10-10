cmake_minimum_required(VERSION 3.18)
project(gpu_stlenmp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB src_files  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(src_file ${src_files})
    get_filename_component(base_name ${src_file} NAME_WE)
    set(executable_name "${base_name}.exe")
    add_executable(${executable_name} ${src_file})
    target_compile_options(${executable_name} PRIVATE -mp=gpu)
    target_link_options(${executable_name} PRIVATE -mp=gpu)
endforeach()
