#include <Kokkos_Core.hpp>
#include <iostream>
#include <random>

int main(int argc, char* argv[]) {
  const int seed{argc > 1 ? std::stoi(argv[1]) : 1234};
  Kokkos::initialize(argc, argv);
  {
    // Create a 1D view of 10 elements
    Kokkos::View<double*> data_dev("data_dev", 10);
    // Create a mirror view
    auto data_host = Kokkos::create_mirror_view(data_dev);
    // Initialize the data on the host using random numbers from mt19937
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (int i = 0; i < data_host.extent(0); ++i) {
      data_host(i) = dist(rng);
    }
    // Deep copy the data from the host to the device
    Kokkos::deep_copy(data_dev, data_host);
    // Print the data
    for (int i = 0; i < data_host.extent(0); ++i) {
      std::cout << data_host(i) << " ";
    }
    // Check which elements of the data are greater than 0.5
    Kokkos::View<int*> mask("mask", data_dev.extent(0));
    Kokkos::parallel_for(
        "filter", data_dev.extent(0),
        KOKKOS_LAMBDA(const int i) { mask(i) = data_dev(i) > 0.5; });
    Kokkos::fence();
    // Perform a parallel scan on the  maks to get the number of elements
    // greater than 0.5 to the left of each element, inclusive; the final value
    // of the scan is stored in count
    int count = 0;
    Kokkos::parallel_scan(
        "scan_mask", mask.extent(0),
        KOKKOS_LAMBDA(const int i, int& update, const bool final) {
          update += mask(i);
          if (final) mask(i) = update;
        },
        count);
    Kokkos::fence();
    std::cout << std::endl
              << "Number of elements greater than 0.5: " << count << std::endl;
    // Create a view to store the filtered data
    Kokkos::View<double*> filtered_data("filtered_data", count);
    // Filter the data
    Kokkos::parallel_for(
        "filter_data", data_dev.extent(0), KOKKOS_LAMBDA(const int i) {
          if (data_dev(i) > 0.5) {
            filtered_data(mask(i) - 1) = data_dev(i);
          }
        });
    Kokkos::fence();
    // Create a mirror view for the filtered data
    auto filtered_data_host = Kokkos::create_mirror_view(filtered_data);
    Kokkos::deep_copy(filtered_data_host, filtered_data);
    // Print the filtered data
    for (int i = 0; i < filtered_data_host.extent(0); ++i) {
      std::cout << filtered_data_host(i) << " ";
    }
    std::cout << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
