#include <Kokkos_Core.hpp>
#include <cmath>
#include <iostream>

using view_type = Kokkos::View<float *>;

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    {
        const int n {argc > 1 ? std::stoi(argv[1]) : 10'000};
        const int N {argc > 2 ? std::stoi(argv[2]) : 20'480'00};

        std::cout << "Kokkos execution space "
            << typeid(Kokkos::DefaultExecutionSpace).name()
            << std::endl;
        view_type a("A", N);
        view_type b("B", N);
        view_type c("C", N);

        float r {2.1f};
        Kokkos::parallel_for("init_vectors", N, KOKKOS_LAMBDA(const int i) {
                a(i) = r*i;
                b(i) = 2.1f;
                c(i) = 0.0f;
                });
        Kokkos::fence();
        Kokkos::parallel_for("multiply_vectors", N, KOKKOS_LAMBDA(const int i) {
                for (int j = 0; j < n; ++j) {
                    c(i) += a(i)*b(i);
                }
                });
        Kokkos::fence();
        float sum {0.0f};
        Kokkos::parallel_reduce("sum_components", N, KOKKOS_LAMBDA(const int i, float& sum) {
                    sum += a(i);
                },
                sum);
        std::cout << "Result: " << sum << std::endl;
    }
    Kokkos::finalize();
}

