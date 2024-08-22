#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  const int max_num_teams = argc > 1 ? atoi(argv[1]) : 1;
  const int max_num_threads = argc > 2 ? atoi(argv[2]) : 1;
  if (argc > 1) {
    printf("Using %d teams and %d thread limit\n", max_num_teams,
           max_num_threads);
#pragma omp target teams num_teams(max_num_teams) thread_limit(max_num_threads)
    {
#pragma omp parallel
      {
#pragma omp single
        {
          const int num_teams = omp_get_num_teams();
          const int num_threads = omp_get_num_threads();
          printf("Number of teams: %d, number of threads: %d\n", num_teams,
                 num_threads);
        }
        const int team_num = omp_get_team_num();
        const int thread_num = omp_get_thread_num();
        printf("Hello from team %d, thread %d\n", team_num, thread_num);
      }
    }
  } else {
    printf("Using default number of teams and thread limit\n");
#pragma omp target teams
    {
#pragma omp parallel
      {
#pragma omp single
        {
          const int num_teams = omp_get_num_teams();
          const int num_threads = omp_get_num_threads();
          printf("Number of teams: %d, number of threads: %d\n", num_teams,
                 num_threads);
        }
        const int team_num = omp_get_team_num();
        const int thread_num = omp_get_thread_num();
        printf("Hello from team %d, thread %d\n", team_num, thread_num);
      }
    }
  }
  return 0;
}
