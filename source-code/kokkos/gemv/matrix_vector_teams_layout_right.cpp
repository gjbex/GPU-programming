#include <Kokkos_Core.hpp>
#include <iostream>

using matrix_view_t = Kokkos::View<float**, Kokkos::LayoutRight>;
using vector_view_t = Kokkos::View<float*>;
using policy_t = Kokkos::TeamPolicy<Kokkos::DefaultExecutionSpace>;

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    const int M{argc > 1 ? std::stoi(argv[1]) : 100};
    const int N{argc > 2 ? std::stoi(argv[2]) : 100};
    matrix_view_t A("A", M, N);
    vector_view_t x("x", N);
    vector_view_t y("y", M);
    Kokkos::parallel_for(
        "init_A", M, KOKKOS_LAMBDA(const int i) {
          for (int j = 0; j < A.extent(1); ++j) {
            A(i, j) = 1.0f;
          }
        });
    Kokkos::parallel_for(
        "init_y", M, KOKKOS_LAMBDA(const int i) { y(i) = 1.0f; });
    Kokkos::parallel_for(
        "init_x", N, KOKKOS_LAMBDA(const int i) { x(i) = 1.0f; });
    Kokkos::fence();
    float result{0.0f};
    Kokkos::parallel_reduce(
        "y*A*x", policy_t(M, Kokkos::AUTO),
        KOKKOS_LAMBDA(const policy_t::member_type& team_member, float& result) {
          const int i{team_member.league_rank()};
          float row_sum{0.0f};
          Kokkos::parallel_reduce(
              Kokkos::TeamThreadRange(team_member, N),
              [=](const int j, float& row_sum) { row_sum += A(i, j) * x(j); },
              row_sum);
          if (team_member.team_rank() == 0) {
            result += y(i) * row_sum;
          }
        },
        result);
    Kokkos::fence();
    std::cout << "y*A*x: " << result << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
