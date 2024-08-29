#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    // Create a view on the host
    Kokkos::View<double*, Kokkos::HostSpace> a("a", 10);

    // Initialize the view on the host
    Kokkos::deep_copy(a, 1.0);

    // Create a mirror view on the host
    auto a_mirror = Kokkos::create_mirror_view(a);

    // Modify the mirror view on the host
    a_mirror(0) = 2.0;

    // Print the original view on the host
    std::cout << "Original view on the host: ";
    for (int i = 0; i < a.extent(0); i++) {
      std::cout << " " << a(i);
    }
    std::cout << std::endl;
    std::cout << "Mirrored view on the host: ";
    for (int i = 0; i < a_mirror.extent(0); i++) {
      std::cout << " " << a_mirror(i);
    }
    std::cout << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
