#include <Kokkos_Core.hpp>
#include <iostream>

using matrix_t = Kokkos::View<double**>;
using vector_t = Kokkos::View<double*>;

int main() {
  const int N{1'000};
  Kokkos::initialize();
  {
    auto device_1 = Kokkos::DefaultExecutionSpace();
    auto device_2 = Kokkos::DefaultExecutionSpace();
    matrix_t A("A", N, N);
    vector_t x("x", N);
    vector_t y_1("y_1", N);
    vector_t y_2("y_2", N);
    Kokkos::deep_copy(device_1, A, 1.0);
    Kokkos::deep_copy(device_1, x, 1.0);
    Kokkos::parallel_for(
        "MatrixVectorProduct", Kokkos::RangePolicy<>(device_1, 0, N),
        KOKKOS_LAMBDA(const int i) {
          double sum = 0.0;
          for (int j = 0; j < N; ++j) {
            sum += A(i, j) * x(j);
          }
          y_1(i) = sum;
        });
    Kokkos::fence();
    Kokkos::deep_copy(device_2, y_2, y_1);
    double sum{0.0};
    Kokkos::parallel_reduce(
        "SumReduction", Kokkos::RangePolicy<>(device_2, 0, N),
        KOKKOS_LAMBDA(const int i, double& update) { update += y_2(i); }, sum);
    std::cout << "Sum: " << sum << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
