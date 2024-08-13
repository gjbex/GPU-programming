#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    // Create a 1D view of size 10
    Kokkos::View<int*> a("a", 10);
    // Fill the view with values 0, 1, 2,...
    Kokkos::parallel_for(
        "init", a.extent(0), KOKKOS_LAMBDA(const int i) { a(i) = i; });
    Kokkos::fence();
    // Create a mirror view to copy the values back to the host
    auto a_host = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(a_host, a);
    // Print the values in the view
    for (int i = 0; i < a_host.extent(0); i++) {
      std::cout << a_host(i) << " ";
    }
    std::cout << std::endl;
    // Create a parallel scan that sums the values in the view
    Kokkos::parallel_scan(
        "sum_scan", a.extent(0),
        KOKKOS_LAMBDA(const int i, int& update, const bool final) {
          update += a(i);
          if (final) a(i) = update;
        });
    Kokkos::fence();
    // Copy the values back to the host
    Kokkos::deep_copy(a_host, a);
    // Print the values in the view
    for (int i = 0; i < a_host.extent(0); i++) {
      std::cout << a_host(i) << " ";
    }
    std::cout << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
