#include <Kokkos_Core.hpp>
#include <iostream>

int main() {
  Kokkos::initialize();
  {
    auto device = Kokkos::DefaultExecutionSpace();
    device.print_configuration(std::cout);
    std::cout << std::endl;
    auto host = Kokkos::DefaultHostExecutionSpace();
    host.print_configuration(std::cout);
  }
  Kokkos::finalize();
  return 0;
}
