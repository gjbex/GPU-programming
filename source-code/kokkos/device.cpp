#include <Kokkos_Core.hpp>
#include <iostream>
#include <typeinfo>

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    std::cout << "Hello World on Kokkos execution space "
        << typeid(Kokkos::DefaultExecutionSpace).name()
        << std::endl;
    Kokkos::finalize();
}

