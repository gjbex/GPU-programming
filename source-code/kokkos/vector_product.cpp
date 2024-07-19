#include <Kokkos_Core.hpp>
#include <cmath>
#include <iostream>

using view_type = Kokkos::View<float *>;

struct InitView {
    private:
        view_type a_;
        view_type b_;
        view_type c_;
        const float r_ {0.2f};

    public:
        explicit InitView(view_type a, view_type b, view_type c) :
            a_ {a}, b_ {b}, c_ {c} {}

        KOKKOS_INLINE_FUNCTION
            void operator()(const int i) const {
                a_(i) = r_*i;
                b_(i) = 2.1f;
                c_(i) = 0.0f;
            }
};

struct VectorProductFunctor {
    private:
        view_type a_;
        view_type b_;
        view_type c_;
        int n_;

    public:
        explicit VectorProductFunctor(view_type a, view_type b, view_type c, int n) :
            a_ {a}, b_ {b}, c_ {c}, n_ {n} {}

        KOKKOS_INLINE_FUNCTION
            void operator()(const int i) const {
                for (int j = 0; j < n_; ++j) {
                    c_(i) += a_(i)*b_(i);
                }
            }
};

struct ReduceFunctor {
    private:
        view_type a_;

    public:
        explicit ReduceFunctor(view_type a) : a_ {a} {}

        KOKKOS_INLINE_FUNCTION
            void operator()(const int i, float &sum) const {
                sum += a_(i);
            }
};

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

        Kokkos::parallel_for("init_vectors", N, InitView(a, b, c));
        Kokkos::fence();
        Kokkos::parallel_for("multiply_vectors", N, VectorProductFunctor(a, b, c, n));
        Kokkos::fence();
        float sum {0.0f};
        Kokkos::parallel_reduce("sum_components", N, ReduceFunctor(c), sum);
        std::cout << "Result: " << sum << std::endl;
    }
    Kokkos::finalize();
}
