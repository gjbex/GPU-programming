cmake_minimum_required(VERSION 3.20)

project(serial LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED COMPONENTS program_options)

file(GLOB src_files  "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
foreach(src_file ${src_files})
    get_filename_component(base_name ${src_file} NAME_WE)
    set(executable_name "${base_name}.exe")
    add_executable(${executable_name} ${src_file})
    target_compile_options(${executable_name} PRIVATE -Wall -Wextra -pedantic)
endforeach()

file(GLOB src_files  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(src_file ${src_files})
    get_filename_component(base_name ${src_file} NAME_WE)
    set(executable_name "${base_name}.exe")
    add_executable(${executable_name} ${src_file})
    target_compile_options(${executable_name} PRIVATE -Wall -Wextra -pedantic)
    target_link_libraries(${executable_name} PRIVATE Boost::program_options)
    target_include_directories(${executable_name} PRIVATE ${Boost_INCLUDE_DIRS})
endforeach()
