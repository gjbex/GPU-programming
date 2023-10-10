#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <numeric>
#include <random>
#include <cstdlib>
#include <sys/time.h>
#include <thrust/device_vector.h>
#include <thrust/sequence.h>
#include <vector>

using my_time_t = std::chrono::microseconds;

struct saxpy_functor {
    private:
        const float a_;

    public:
        saxpy_functor(float a) : a_(a) {}

        __host__ __device__
        float operator()(const float& x, const float& y) const {
            return a_*x + y;
        }
};

long time_operation(std::function<void()> f) {
    auto start_time = std::chrono::steady_clock::now();
    f();
    auto end_time = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<my_time_t>(end_time - start_time).count();
}

float compute_time(struct timeval start_time, struct timeval end_time) {
    return (end_time.tv_sec - start_time.tv_sec) +
                1.0e-6*(end_time.tv_usec - start_time.tv_usec);
}

float thrust_compute(thrust::device_vector<float>& x,
                      thrust::device_vector<float>& y) {
    for (float a = 1.0e-5f; a < 1.0e-2f; a += 1.0e-5f)
        thrust::transform(x.begin(), x.end(), y.begin(), y.begin(),
                          saxpy_functor(a));
    float init {0.0f};
    thrust::plus<float> operation;
    return thrust::reduce(y.begin(), y.end(), init, operation);
}

float host_compute(std::vector<float>& x, std::vector<float>& y) {
    for (float a = 1.0e-5f; a < 1.0e-2f; a += 1.0e-5f)
        for (int i = 0; i < x.size(); i++)
            y[i] += a*x[i];
    float result {0.0f};
    for (int i = 0; i < y.size(); i++)
        result += y[i];
    return result;
}

int main(int argc, char *argv[]) {
    const bool verbose {false};
    struct timeval start_time, end_time;
    int n {5};
    if (argc > 1)
        n = atoi(argv[1]);
    std::vector<float> x_host(n);
    std::vector<float> y_host(n);
    for (int i = 0; i < x_host.size(); i++) {
        x_host[i] = i + 1.0;
        y_host[i] = i + 10.0;
    }
    gettimeofday(&start_time, NULL);
    thrust::device_vector<float> x {x_host};
    thrust::device_vector<float> y {y_host};
    gettimeofday(&end_time, NULL);
    std::cout << "data transfer time = " << compute_time(start_time, end_time)
         << std::endl;
    if (verbose) {
        std::cout << "x (GPU)" << std::endl;
        thrust::copy(x.begin(), x.end(),
                std::ostream_iterator<float>(std::cout, "\n"));
        std::cout << "y (GPU)" << std::endl;
        thrust::copy(y.begin(), y.end(),
                std::ostream_iterator<float>(std::cout, "\n"));
    }
    gettimeofday(&start_time, NULL);
    float thrust_result = thrust_compute(x, y);
    gettimeofday(&end_time, NULL);
    std::cout << "GPU compute time = " << compute_time(start_time, end_time)
         << std::endl;
    if (verbose) {
        std::cout << "GPU result" << std::endl;
        thrust::copy(y.begin(), y.end(),
                     std::ostream_iterator<float>(std::cout, "\n"));
    }
    std::cout << "GPU sum = " << thrust_result << std::endl;
    gettimeofday(&start_time, NULL);
    float host_result = host_compute(x_host, y_host);
    gettimeofday(&end_time, NULL);
    std::cout << "CPU compute time = " << compute_time(start_time, end_time)
         << std::endl;
    std::cout << "CPU sum = " << host_result << std::endl;
    return 0;
}
