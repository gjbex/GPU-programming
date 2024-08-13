#include <Kokkos_Core.hpp>
#include <iostream>

using policy_t = Kokkos::TeamPolicy<>;
using member_t = policy_t::member_type;

int main(int argc, char* argv[]) {
  Kokkos::initialize(argc, argv);
  {
    const int nr_of_teams{argc > 1 ? std::stoi(argv[1]) : 2};
    const int team_size{argc > 2 ? std::stoi(argv[2]) : 3};
    const int loop_size{argc > 3 ? std::stoi(argv[3]) : 16};
    policy_t policy(nr_of_teams, team_size);
    std::cout << "Number of teams: " << policy.league_size() << std::endl;

    std::cout << "parallel_for over league members, team members" << std::endl;
    Kokkos::parallel_for(
        policy, KOKKOS_LAMBDA(const member_t& thread) {
          Kokkos::printf("League rank %d out of %d, thread rank %d out of %d\n",
                         thread.league_rank(), thread.league_size(),
                         thread.team_rank(), thread.team_size());
        });
    Kokkos::fence();
    std::cout
        << "parallel_for over league members, team members, single per team"
        << std::endl;
    Kokkos::parallel_for(
        policy, KOKKOS_LAMBDA(const member_t& thread) {
          Kokkos::single(Kokkos::PerTeam(thread), [&]() {
            Kokkos::printf(
                "League rank %d out of %d, thread rank %d out of %d\n",
                thread.league_rank(), thread.league_size(), thread.team_rank(),
                thread.team_size());
          });
        });
    Kokkos::fence();
    std::cout
        << "parallel_for over league members, team members, single per thread"
        << std::endl;
    Kokkos::parallel_for(
        policy, KOKKOS_LAMBDA(const member_t& thread) {
          Kokkos::single(Kokkos::PerThread(thread), [&]() {
            Kokkos::printf(
                "League rank %d out of %d, thread rank %d out of %d\n",
                thread.league_rank(), thread.league_size(), thread.team_rank(),
                thread.team_size());
          });
        });
  }
  Kokkos::finalize();
  return 0;
}
