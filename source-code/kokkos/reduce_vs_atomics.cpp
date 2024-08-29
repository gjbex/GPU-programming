#include <Kokkos_Core.hpp>
#include <chrono>
#include <iostream>

int main(int argc, char* argv[]) {
  const long N{argc > 1 ? std::stol(argv[1]) : 5'000'000};
  Kokkos::initialize(argc, argv);
  {
    long sum{0};
    auto start = std::chrono::high_resolution_clock::now();
    Kokkos::parallel_reduce(
        Kokkos::RangePolicy<Kokkos::IndexType<long>>(0, N),
        KOKKOS_LAMBDA(const long i, long& lsum) { lsum += i; }, sum);
    Kokkos::fence();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "reduce sum: " << sum << std::endl;
    std::cout << "reduce time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << " ms" << std::endl;
    Kokkos::View<long*> sum_view("sum", 1);
    Kokkos::deep_copy(sum_view, 0);
    auto host_sum_view = Kokkos::create_mirror_view(sum_view);
    start = std::chrono::high_resolution_clock::now();
    Kokkos::parallel_for(
        Kokkos::RangePolicy<Kokkos::IndexType<long>>(0, N),
        KOKKOS_LAMBDA(const long i) { Kokkos::atomic_add(&sum_view(0), i); });
    Kokkos::fence();
    end = std::chrono::high_resolution_clock::now();
    Kokkos::deep_copy(host_sum_view, sum_view);
    std::cout << "atomic sum: " << host_sum_view(0) << std::endl;
    std::cout << "atomic time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << " ms" << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
