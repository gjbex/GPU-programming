#include <iostream>
#include <Kokkos_Core.hpp>
#include <Kokkos_Random.hpp>

int main(int argc, char* argv[]) {
    Kokkos::initialize(argc, argv);
    {
        // Create a random number generator
        using Random = Kokkos::Random_XorShift1024_Pool<Kokkos::DefaultExecutionSpace>;
        Random random(1234);

        // Create a view of random numbers
        Kokkos::View<double*> data("data", 10);
        Kokkos::fill_random(data, random, 0.0, 1.0);

        // Create a mirror view to print the data
        auto data_host = Kokkos::create_mirror_view(data);
        deep_copy(data_host, data);
        for (int i = 0; i < data_host.size(); i++) {
            std::cout << data_host(i) << " ";
        }
        std::cout << std::endl;

        // Find the minimum value and its location
        double min_val {};
        Kokkos::parallel_reduce("minloc", data.size(), KOKKOS_LAMBDA(int i, double& lmin_val) {
                    if (data(i) < lmin_val) {
                        lmin_val = data(i);
                    }
                },
                Kokkos::Min<double, Kokkos::HostSpace>(min_val));

        // Print the result
        std::cout << "Minimum value: " << min_val << std::endl;
    }
    Kokkos::finalize();
    return 0;
}
