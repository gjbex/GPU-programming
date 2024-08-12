#include <iostream>
#include <Kokkos_Core.hpp>

using policy_t = Kokkos::TeamPolicy<>;
using member_t = policy_t::member_type;

int main(int argc, char *argv[]) {
    Kokkos::initialize(argc, argv);
    {
        const int nr_of_teams {argc > 1 ? std::stoi(argv[1]) : 2};
        const int team_size {argc > 2 ? std::stoi(argv[2]) : 3};
        const int loop_size {argc > 3 ? std::stoi(argv[3]) : 6};
        policy_t policy(nr_of_teams, team_size);
        std::cout << "Number of teams: " << policy.league_size() << std::endl;

        std::cout << "parallel_for over league members, team members" << std::endl;
        Kokkos::parallel_for(policy, KOKKOS_LAMBDA(const member_t& thread) {
                Kokkos::printf("League rank %d out of %d, thread rank %d out of %d\n",
                        thread.league_rank(), thread.league_size(),
                        thread.team_rank(), thread.team_size());
                });
        Kokkos::fence();
        std::cout << "parallel_for over league members, parallel_for of team members over loop of size " << loop_size << std::endl;
        Kokkos::parallel_for(policy, KOKKOS_LAMBDA(const member_t& thread) {
                Kokkos::parallel_for(Kokkos::TeamThreadRange(thread, loop_size), [=] (const int i) {
                        Kokkos::printf("League rank %d out of %d, thread rank %d out of %d: %d\n",
                                thread.league_rank(), thread.league_size(),
                                thread.team_rank(), thread.team_size(),
                                i);
                        });
                });
        Kokkos::fence();
        std::cout << "parallel_for over league members, parallel_reduce of team members over loop of size " << loop_size << std::endl;
        Kokkos::parallel_for(policy, KOKKOS_LAMBDA(const member_t& thread) {
                int sum {0};
                Kokkos::parallel_reduce(Kokkos::TeamThreadRange(thread, loop_size), [=] (const int i, int& lsum) {
                        lsum += i;
                        }, sum);
                Kokkos::printf("League rank %d out of %d, thread rank %d out of %d: %d\n",
                        thread.league_rank(), thread.league_size(),
                        thread.team_rank(), thread.team_size(),
                        sum);
                });
    }
    Kokkos::finalize();
    return 0;
}
