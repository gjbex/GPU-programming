#include <Kokkos_Core.hpp>
#include <iostream>

int main() {
  const int nr_rows{10};
  const int nr_cols{15};
  Kokkos::initialize();
  {
    Kokkos::View<int**> a("A", nr_rows, nr_cols);
    Kokkos::parallel_for(
        "init_A",
        Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0, 0}, {nr_rows, nr_cols}),
        KOKKOS_LAMBDA(const int i, const int j) {
          a(i, j) = 1 + i * nr_cols + j;
        });
    auto a_host = Kokkos::create_mirror_view(a);
    Kokkos::deep_copy(a_host, a);
    for (int i = 0; i < a_host.extent(0); i++) {
      for (int j = 0; j < a_host.extent(1); j++) {
        std::cout << a_host(i, j) << " ";
      }
      std::cout << "\n";
    }
    Kokkos::fence();
    auto a_subview = Kokkos::subview(a, Kokkos::ALL, std::make_pair(2, 5));
    auto a_subview_host = Kokkos::create_mirror_view(a_subview);
    Kokkos::deep_copy(a_subview_host, a_subview);
    for (int i = 0; i < a_subview_host.extent(0); i++) {
      for (int j = 0; j < a_subview_host.extent(1); j++) {
        std::cout << a_subview_host(i, j) << " ";
      }
      std::cout << "\n";
    }
  }
  Kokkos::finalize();
  return 0;
}
