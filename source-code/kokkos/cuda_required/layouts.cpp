#include <Kokkos_Core.hpp>
#include <iostream>

using ll_view_t = Kokkos::View<int[5][3], Kokkos::LayoutLeft>;
using lr_view_t = Kokkos::View<int[5][3], Kokkos::LayoutRight>;
using cpu_view_t = Kokkos::View<int[5][3], Kokkos::HostSpace>;
using gpu_view_t = Kokkos::View<int[5][3], Kokkos::CudaSpace>;

int main() {
  Kokkos::initialize();
  {
    ll_view_t ll_view("ll_view");
    lr_view_t lr_view("lr_view");
    std::cout << "5 x 3 array of ints" << std::endl;
    std::cout << "LeftLayout view strides:  " << ll_view.stride(0) << ", "
              << ll_view.stride(1) << std::endl;
    std::cout << "RightLayour view strides: " << lr_view.stride(0) << ", "
              << lr_view.stride(1) << std::endl;
    std::cout << "HostSpace view strides:    "
              << cpu_view_t("cpu_view").stride(0) << ", "
              << cpu_view_t("cpu_view").stride(1) << std::endl;
    std::cout << "CudaSpace view strides:    "
              << gpu_view_t("gpu_view").stride(0) << ", "
              << gpu_view_t("gpu_view").stride(1) << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
