file(GLOB src_files  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(src_file ${src_files})
    get_filename_component(base_name ${src_file} NAME_WE)
    set(executable_name "${base_name}.exe")
    add_executable(${executable_name} ${src_file})
    target_include_directories(${executable_name} PRIVATE Kokkos::kokkos)
    target_link_libraries(${executable_name} Kokkos::kokkos)
    target_compile_options(${executable_name} PRIVATE -Wall -Wextra -pedantic -Werror)
endforeach()
