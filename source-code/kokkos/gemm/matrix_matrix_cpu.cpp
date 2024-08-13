#include <Kokkos_Core.hpp>
#include <iostream>

using matrix_view_t = Kokkos::View<float**, Kokkos::HostSpace>;

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    const int N{argc > 1 ? std::atoi(argv[1]) : 100};
    const int nr_iter{argc > 2 ? std::atoi(argv[2]) : 1};
    matrix_view_t A("A", N, N);
    matrix_view_t B("B", N, N);
    Kokkos::parallel_for(
        "init_A_B",
        Kokkos::MDRangePolicy<Kokkos::OpenMP, Kokkos::Rank<2>>({0, 0}, {N, N}),
        KOKKOS_LAMBDA(const int i, const int j) {
          A(i, j) = 1.0f;
          B(i, j) = 0.0f;
        });
    Kokkos::fence();
    for (int iter = 0; iter < nr_iter; ++iter) {
      Kokkos::parallel_for(
          "gemm",
          Kokkos::MDRangePolicy<Kokkos::OpenMP, Kokkos::Rank<2>>({0, 0},
                                                                 {N, N}),
          KOKKOS_LAMBDA(const int i, const int j) {
            for (int k = 0; k < N; ++k) {
              B(i, j) += A(i, k) * A(k, j);
            }
          });
      Kokkos::fence();
    }
    float sum = 0.0f;
    Kokkos::parallel_reduce(
        "sum_B",
        Kokkos::MDRangePolicy<Kokkos::OpenMP, Kokkos::Rank<2>>({0, 0}, {N, N}),
        KOKKOS_LAMBDA(const int i, const int j, float& lsum) {
          lsum += B(i, j);
        },
        sum);
    std::cout << "Sum of B: " << sum << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
