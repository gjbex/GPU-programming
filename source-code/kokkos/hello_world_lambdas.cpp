#include <Kokkos_Core.hpp>
#include <iostream>
#include <typeinfo>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  Kokkos::parallel_for(
      15, KOKKOS_LAMBDA(const int i) {
        Kokkos::printf("Hello World from i = %d\n", i);
      });
  Kokkos::finalize();
}
