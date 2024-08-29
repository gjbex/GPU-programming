#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  const int max_steps{argc > 1 ? std::stoi(argv[1]) : 1000};
  const int nr_walks{argc > 2 ? std::stoi(argv[2]) : 2000};
  Kokkos::initialize(argc, argv);
  {
    // Create a random number generator
    using RandomGenerator =
        Kokkos::Random_XorShift64_Pool<Kokkos::DefaultExecutionSpace>;
    RandomGenerator random_pool(1234);

    // Create a view to store the distribution of the distance from the origin
    Kokkos::View<int*> distance("distance", 2 * max_steps + 1);
    Kokkos::deep_copy(distance, 0);

    // Set memory traits to ranodm access and atomic
    // This is necessary because multiple threads will write to the same memory
    // location
    Kokkos::View<int*,
                 Kokkos::MemoryTraits<Kokkos::RandomAccess | Kokkos::Atomic>>
        distance_atomic = distance;

    // Perform the random walks
    Kokkos::parallel_for(
        "walks", nr_walks, KOKKOS_LAMBDA(const int walk) {
          auto generator = random_pool.get_state();
          int pos = max_steps;
          for (int step = 0; step < (walk % 2 == 0 ? max_steps : max_steps - 1);
               ++step) {
            pos += generator.drand() < 0.5 ? -1 : 1;
          }
          distance_atomic(pos)++;
        });

    // Check that distance contains the correct values
    int nr_walks_check{0};
    Kokkos::parallel_reduce(
        "check", distance_atomic.extent(0),
        KOKKOS_LAMBDA(const int i, int& nr_walks_check) {
          nr_walks_check += distance_atomic(i);
        },
        nr_walks_check);
    // Print the distribution of the distance from the origin
    auto distance_host = Kokkos::create_mirror_view(distance);
    Kokkos::deep_copy(distance_host, distance);
    Kokkos::fence();
    for (int steps = -max_steps; steps <= max_steps; ++steps) {
      std::cout << steps << " " << distance_host(steps + max_steps) << "\n";
    }
    std::cerr << "# Total number of walks: " << nr_walks_check << "\n";
  }
  Kokkos::finalize();
  return 0;
}
