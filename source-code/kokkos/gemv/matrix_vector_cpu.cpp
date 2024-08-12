#include <iostream>
#include <Kokkos_Core.hpp>

using matrix_view_t = Kokkos::View<float**, Kokkos::HostSpace>;
using vector_view_t = Kokkos::View<float*, Kokkos::HostSpace>;
using policy_t = Kokkos::RangePolicy<Kokkos::OpenMP>;

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    {
        const int M {argc > 1 ? std::stoi(argv[1]) : 100};
        const int N {argc > 2 ? std::stoi(argv[2]) : 100};
        matrix_view_t A("A", M, N);
        vector_view_t x("x", N);
        vector_view_t y("y", M);
        Kokkos::parallel_for("init_A", policy_t(0, M),
                KOKKOS_LAMBDA(const int i) {
                for (int j = 0; j < A.extent(1); ++j) {
                    A(i, j) = 1.0f;
                }
                });
        Kokkos::parallel_for("init_y", policy_t(0, M),
                KOKKOS_LAMBDA(const int i) {
                y(i) = 1.0f;
                });
        Kokkos::parallel_for("init_x", policy_t(0, N),
                KOKKOS_LAMBDA(const int i) {
                x(i) = 1.0f;
                });
        Kokkos::fence();
        float result {0.0f};
        Kokkos::parallel_reduce("y*A*x", policy_t(0, M),
                KOKKOS_LAMBDA(const int i, float& lsum) {
                float sum {0.0f};
                for (int j = 0; j < A.extent(1); ++j) {
                    sum += A(i, j)*x(j);
                }
                lsum += y(i)*sum;
                }, result);
        Kokkos::fence();
        std::cout << "Sum of y: " << result << std::endl;
    }
    Kokkos::finalize();
    return 0;
}
