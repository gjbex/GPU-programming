#include <iostream>
#include <Kokkos_Core.hpp>

using matrix_view_t = Kokkos::View<float**>;
using vector_view_t = Kokkos::View<float*>;

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    {
        const int M {argc > 1 ? std::stoi(argv[1]) : 100};
        const int N {argc > 2 ? std::stoi(argv[2]) : 100};
        matrix_view_t A("A", M, N);
        vector_view_t x("x", N);
        vector_view_t y("y", M);
        Kokkos::parallel_for("init_A", M, KOKKOS_LAMBDA(const int i) {
            for (int j = 0; j < A.extent(1); ++j) {
                A(i, j) = 1.0f;
            }
        });
        Kokkos::parallel_for("init_y", M, KOKKOS_LAMBDA(const int i) {
            y(i) = 1.0f;
        });
        Kokkos::parallel_for("init_x", N, KOKKOS_LAMBDA(const int i) {
            x(i) = 1.0f;
        });
        Kokkos::fence();
        float result {0.0f};
        Kokkos::parallel_reduce("y*A*x", M, KOKKOS_LAMBDA(const int i, float& lsum) {
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
