#include <Kokkos_Core.hpp>
#include <Kokkos_DualView.hpp>
#include <cmath>
#include <iostream>

using view_type = Kokkos::DualView<double*>;

void init_data(view_type data) {
  data.sync_device();
  auto device_view{data.d_view};
  auto n{data.extent(0)};
  const double pi{std::acos(-1.0)};
  Kokkos::parallel_for(
      "init", n, KOKKOS_LAMBDA(int i) { device_view(i) = sin((pi * i) / n); });

  data.modify_device();
}

double quad(view_type data) {
  data.sync_host();
  auto host_view{data.h_view};
  double delta{1.0 / data.extent(0)};
  double sum{0.0};
  for (int i = 0; i < data.extent(0); ++i) {
    sum += host_view(i);
  }
  return delta * sum;
}

void normalize(view_type data, const double factor) {
  data.sync_device();
  auto device_view{data.d_view};
  auto n{data.extent(0)};
  Kokkos::parallel_for(
      "normalize", n, KOKKOS_LAMBDA(int i) { device_view(i) /= factor; });
  data.modify_device();
}

int main(int argc, char* argv[]) {
  const int n{argc > 1 ? std::atoi(argv[1]) : 1000};
  Kokkos::initialize(argc, argv);
  {
    // Create a DualView with 10 elements
    Kokkos::DualView<double*> dual_view("dual_view", n);

    // Initialize the data
    init_data(dual_view);

    // Compute the integral
    double integral = quad(dual_view);
    std::cout << "Integral before normalization: " << integral << std::endl;

    // Normalize the data
    normalize(dual_view, integral);

    // Compute the integral again
    integral = quad(dual_view);
    std::cout << "Integral after normalization: " << integral << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
