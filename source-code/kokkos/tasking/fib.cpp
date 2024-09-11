#include <iostream>
#include <limits>
#include <Kokkos_Core.hpp>

using value_type = long;

size_t estimate_required_memory(value_type n) {
  auto nl = static_cast<size_t>(n);
  return (nl + 1) * (nl + 1) * 2000;
}

template <class Scheduler>
struct FibonacciTask
{
using value_type = long;
  using future_type = Kokkos::BasicFuture<value_type, Scheduler>;

  value_type n_;
  future_type future_n_1;
  future_type future_n_2;

  KOKKOS_INLINE_FUNCTION
  explicit FibonacciTask(value_type n) noexcept : n_(n) {}

  template <class TeamMember>
  KOKKOS_INLINE_FUNCTION
  void operator()(TeamMember& member, value_type& result) {
    auto& scheduler = member.scheduler();
    if(n_ < 2) {
      result = n_;
    } else if(!future_n_1.is_null() && !future_n_2.is_null()) {
        // Both futrues are ready
      result = future_n_1.get() + future_n_2.get();
    } else {
      // Spawn child tasks for the subproblems
      future_n_1 = Kokkos::task_spawn(
        Kokkos::TaskSingle(scheduler),
        FibonacciTask{n_ - 1}
      );
      future_n_2 = Kokkos::task_spawn(
        Kokkos::TaskSingle(scheduler),
        FibonacciTask{n_ - 2}
      );
      
      // Create an aggregate predecessor for our respawn
      Kokkos::BasicFuture<void, Scheduler> fib_array[] = { future_n_1, future_n_2 };
      auto f_all = scheduler.when_all(fib_array, 2);

      // Respawn this task with `f_all` as a predecessor
      Kokkos::respawn(this, f_all);
    }
  }

};

int main(int argc, char* argv[]) {
  const value_type n {argc > 1 ? std::stol(argv[1]) : 10};
  if(n < 0) {
      std::cerr << "Argument to Fibonacci must be non-negative.\n";
    std::exit(1);
  }

  Kokkos::initialize(argc, argv);
  {
    using scheduler_type = Kokkos::TaskScheduler<Kokkos::DefaultExecutionSpace>;
    using memory_space = typename scheduler_type::memory_space;
    using memory_pool_type = typename scheduler_type::memory_pool;

    auto memory_pool = memory_pool_type(memory_space{}, estimate_required_memory(n));
    auto scheduler = scheduler_type(memory_pool);

    Kokkos::BasicFuture<value_type, scheduler_type> result;

    Kokkos::Timer timer;
    {
      // launch the root task from the host
      result =
        Kokkos::host_spawn(
          Kokkos::TaskSingle(scheduler),
          FibonacciTask<scheduler_type>{n}
        );

      // wait on all tasks submitted to the scheduler to be done
      Kokkos::wait(scheduler);
    }
    auto time = timer.seconds();

    // Output results
    if(!result.is_null() && result.is_ready()) {
        std::cout << "fib(" << n << ") = " << result.get() << std::endl;
    } else {
        std::cerr << "Error! Result of Fibonacci(" << n << ") is not ready\n";
    }

    // Print the timing results
    printf("  Task computation executed in %g s\n", time);
  }
  Kokkos::finalize();

  return 0;
}
