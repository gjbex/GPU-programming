find_package(Boost REQUIRED COMPONENTS program_options)

add_library(molecular_dynamis
    data_types.cpp particles.cpp abstract_measures.cpp measures.cpp
    stepper.cpp simulation.cpp)
target_compile_options(molecular_dynamis PRIVATE -Wall -Wextra -pedantic)

add_library(cli cli.cpp)
target_compile_options(cli PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(cli PRIVATE Boost::program_options)
target_include_directories(cli PRIVATE Boost::program_options)

add_executable(molecular_dynamics.exe main.cpp)
target_compile_options(molecular_dynamics.exe PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(molecular_dynamics.exe PRIVATE molecular_dynamis)
target_link_libraries(molecular_dynamics.exe PRIVATE cli)

add_executable(two_particles.exe two_particles.cpp)
target_compile_options(two_particles.exe PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(two_particles.exe PRIVATE molecular_dynamis)
target_link_libraries(two_particles.exe PRIVATE cli)
