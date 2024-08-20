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
    Kokkos::TeamPolicy<> policy(nr_elements, Kokkos::AUTO);
    using ScratchView =
        Kokkos::View<double*,
                     Kokkos::DefaultExecutionSpace::scratch_memory_space>;
    size_t scratch_size = ScratchView::shmem_size(vector_size);
    const int level{0};
    Kokkos::parallel_for(
        "C=A*B", policy.set_scratch_size(level, Kokkos::PerTeam(scratch_size)),
        KOKKOS_LAMBDA(const Kokkos::TeamPolicy<>::member_type& team) {
          const int element{team.league_rank()};
          ScratchView scratch(team.team_scratch(level), vector_size);
          Kokkos::parallel_for(
              Kokkos::TeamVectorRange(team, vector_size),
              [&](const int& i) { scratch(i) = B(element, i); });
          team.team_barrier();
          Kokkos::parallel_for(Kokkos::TeamThreadRange(team, nr_qp),
                               [&](const int& qp) {
                                 double total{0.0};
                                 for (int i = 0; i < vector_size; ++i) {
                                   total += A(element, qp, i) * scratch(i);
                                 }
                                 C(element, qp) = total;
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
