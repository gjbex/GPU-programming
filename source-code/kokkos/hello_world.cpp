#include <Kokkos_Core.hpp>
#include <iostream>
#include <typeinfo>

struct hello_world {
    KOKKOS_INLINE_FUNCTION
        void operator()(const int i) const {
            Kokkos::printf("Hello from i = %i\n", i);
        }
};

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    std::cout << "Hello World on Kokkos execution space "
        << typeid(Kokkos::DefaultExecutionSpace).name()
        << std::endl;
    Kokkos::parallel_for("HelloWorld", 15, hello_world());
    Kokkos::finalize();
}
