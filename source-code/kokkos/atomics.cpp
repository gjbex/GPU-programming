#include <Kokkos_Core.hpp>
#include <iostream>

int main() {
  const int N{1'000'000'000};
  Kokkos::initialize();
  {
    Kokkos::View<int*> view("view", 1);
    auto host_view = Kokkos::create_mirror_view(view);
    Kokkos::deep_copy(view, 0);
    Kokkos::parallel_for(
        N, KOKKOS_LAMBDA(const int i) { view(0)++; });
    Kokkos::fence();
    Kokkos::deep_copy(host_view, view);
    std::cout << "no atomics view(0) = " << host_view(0) << "/" << N
              << std::endl;
    Kokkos::deep_copy(view, 0);
    Kokkos::parallel_for(
        N, KOKKOS_LAMBDA(const int i) { Kokkos::atomic_increment(&view(0)); });
    Kokkos::fence();
    Kokkos::deep_copy(host_view, view);
    std::cout << "atomics view(0) = " << host_view(0) << "/" << N << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
