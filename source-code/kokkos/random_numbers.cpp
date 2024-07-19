#include <Kokkos_Core.hpp>
#include <iostream>
#include <random>
#include <vector>

const int nr_distr {5};
using view_type = Kokkos::View<float * [nr_distr]>;
using host_view_type = view_type::HostMirror;

struct MultiSum {
    using value_type = float[];
    using size_type = Kokkos::View<float * [nr_distr]>::size_type;

    private:
        view_type data_;

    public:
        size_type value_count;
        explicit MultiSum(view_type data) : data_(data), value_count {data.extent(1)} {}
        KOKKOS_INLINE_FUNCTION
            void init(value_type result) const {
                for (int j = 0; j < value_count; ++j) {
                    result[j] = 0.0f;
                }
            }
        KOKKOS_INLINE_FUNCTION
            void operator()(const size_type i, value_type result) const {
                for (int j = 0; j < value_count; ++j) {
                    result[j] += data_(i, j);
                }
            }
        KOKKOS_INLINE_FUNCTION
            void join(value_type result1, const value_type result2) const {
                for (int j = 0; j < value_count; ++j) {
                    result1[j] += result2[j];
                }
            }
};

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    {
        const int N {argc > 1 ? std::stoi(argv[1]) : 1'000};
        const long seed {1234};
        std::mt19937 engine(seed);
        view_type data("data", N);
        host_view_type host_data {Kokkos::create_mirror_view(data)};
        for (int j = 0; j < nr_distr; ++j) {
            std::uniform_real_distribution<float> distr(-3.0f + j, -1.0f + j);
            for (int i = 0; i < N; ++i) {
                host_data(i, j) = distr(engine);
            }
        }
        Kokkos::deep_copy(data, host_data);
        float result[nr_distr] {0.0f};
        Kokkos::parallel_reduce(N, MultiSum(data), result);
        for (int j = 0; j < nr_distr; ++j) {
            std::cout << j << ": " << result[j]/N << "\n";
        }
    }
    Kokkos::finalize();
}
