#include <Kokkos_Core.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  const int nr_elements{argc > 1 ? std::stoi(argv[1]) : 10};
  const int nr_qp{argc > 2 ? std::stoi(argv[2]) : 15};
  const int vector_size{argc > 3 ? std::stoi(argv[3]) : 5};
  Kokkos::initialize(argc, argv);
  {
    // Create and initialize A
    Kokkos::View<double***> A("A", nr_elements, nr_qp, vector_size);
    Kokkos::deep_copy(A, 1.0);
    // Create and initialize B
    Kokkos::View<double**> B("B", nr_elements, vector_size);
    Kokkos::deep_copy(B, 2.0);
    // Create C
    Kokkos::View<double**> C("C", nr_elements, nr_qp);

    // Compute C = A * B
    Kokkos::parallel_for(
        "C=A*B", Kokkos::TeamPolicy<>(nr_elements, Kokkos::AUTO),
        KOKKOS_LAMBDA(const Kokkos::TeamPolicy<>::member_type& team) {
          Kokkos::parallel_for(
              Kokkos::TeamThreadRange(team, nr_qp), [&](const int& qp) {
                double total{0.0};
                Kokkos::parallel_reduce(
                    Kokkos::ThreadVectorRange(team, vector_size),
                    [&](const int& i, double& update) {
                      update += A(team.league_rank(), qp, i) *
                                B(team.league_rank(), i);
                    },
                    total);
                C(team.league_rank(), qp) = total;
              });
        });

    // Compute the sum of all elements in C
    double sum{0.0};
    Kokkos::parallel_reduce(
        "sum",
        Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0, 0}, {nr_elements, nr_qp}),
        KOKKOS_LAMBDA(int element, int qp, double& update) {
          update += C(element, qp);
        },
        sum);

    // Print the sum
    std::cout << "Sum of all elements in C: " << sum << std::endl;
  }
  Kokkos::finalize();
  return 0;
}
