#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>
#include <iostream>

struct ComputePi {
 private:
  const int N_;
  Kokkos::Random_XorShift64_Pool<> rand_pool;

 public:
  ComputePi(int N, unsigned long seed) : N_{N}, rand_pool{seed} {}
  KOKKOS_INLINE_FUNCTION
  void operator()(const int i, float& lsum) const {
    auto rand_gen = rand_pool.get_state();
    for (int j = 0; j < N_; ++j) {
      float x{rand_gen.frand()};
      float y{rand_gen.frand()};
      if (x * x + y * y <= 1.0f) {
        lsum += 1.0f;
      }
    }
    rand_pool.free_state(rand_gen);
  }
};

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    int n{argc > 1 ? std::atoi(argv[1]) : 1'000};
    int N{argc > 2 ? std::atoi(argv[2]) : 1'000};
    float pi{0.0f};
    Kokkos::parallel_reduce(n, ComputePi(N, 1234), pi);
    pi = 4.0 * pi / (N * n);
    std::cout << "PI = " << pi << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
