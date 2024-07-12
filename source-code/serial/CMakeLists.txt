cmake_minimum_required(VERSION 3.20)

project(serial LANGUAGES C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB src_files  "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
foreach(src_file ${src_files})
    get_filename_component(base_name ${src_file} NAME_WE)
    set(executable_name "${base_name}.exe")
    add_executable(${executable_name} ${src_file})
    target_compile_options(${executable_name} PRIVATE -Wall -Wextra -pedantic -Werror)
endforeach()
