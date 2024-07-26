find_package(Boost REQUIRED COMPONENTS program_options)

add_library(molecular_dynamis
    data_types.cpp particles.cpp abstract_measures.cpp measures.cpp
    stepper.cpp simulation.cpp)
add_executable(molecular_dynamics.exe
    main.cpp cli.cpp)
target_compile_options(molecular_dynamics.exe PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(molecular_dynamics.exe PRIVATE Boost::program_options)
target_link_libraries(molecular_dynamics.exe PRIVATE molecular_dynamis)
target_include_directories(molecular_dynamics.exe PRIVATE Boost::program_options)
# target_include_directories(molecular_dynamics.exe PRIVATE ${Boost_INCLUDE_DIRS})
