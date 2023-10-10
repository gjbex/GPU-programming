#include <chrono>
#include <iostream>
#include <numeric>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/generate.h>
#include <thrust/host_vector.h>
#include <thrust/reduce.h>
#include <random>

using my_time_t = std::chrono::microseconds;

int main(int argc, char* argv[]) {
    int size {10};
    if (argc > 1) {
        size = std::stoi(argv[1]);
    }
    // set up random number generator and distribution
    std::mt19937 engine(1234);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    // generate random data on the host
    thrust::host_vector<float> h_vec(size);
    thrust::generate(h_vec.begin(), h_vec.end(),
            [&engine, &dist] () { return dist(engine); });

    // compute the sum of the elements on the host
    {
        auto start_time = std::chrono::steady_clock::now();
        auto sum = std::reduce(h_vec.cbegin(), h_vec.cend());
        std::cout << "host sum is " << sum << "\n";
        auto end_time = std::chrono::steady_clock::now();
        auto delta_time = std::chrono::duration_cast<my_time_t>(end_time - start_time);
        std::cout << "host sum took " << delta_time.count() << " us\n";
    }

    {
        auto start_time = std::chrono::steady_clock::now();
        // transfer to device and compute sum
        thrust::device_vector<long> d_vec = h_vec;

        // initial value of the reduction
        float init {0.0f}; 
     
        // binary operation used to reduce values
        thrust::plus<float> binary_op;

        // compute sum on the device
        auto sum = thrust::reduce(d_vec.begin(), d_vec.end(), init, binary_op);

        // print the sum
        std::cout << "device sum is " << sum << std::endl;
        auto end_time = std::chrono::steady_clock::now();
        auto delta_time = std::chrono::duration_cast<my_time_t>(end_time - start_time);
        std::cout << "device sum took " << delta_time.count() << " us\n";
    }

    return 0;
}
