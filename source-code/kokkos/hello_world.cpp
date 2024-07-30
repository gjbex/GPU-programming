#include <Kokkos_Core.hpp>
#include <iostream>

struct hello_world {
    KOKKOS_INLINE_FUNCTION
        void operator()(const int i) const {
            Kokkos::printf("Hello from i = %d\n", i);
        }
};

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    Kokkos::parallel_for("HelloWorld", 15, hello_world());
    Kokkos::finalize();
}
