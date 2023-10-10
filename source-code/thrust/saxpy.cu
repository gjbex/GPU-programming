#include <algorithm>
#include <chrono>
#include <iostream>
#include <iterator>
#include <vector>
#include <thrust/device_vector.h>
#include <thrust/functional.h>
#include <thrust/host_vector.h>
#include <thrust/transform.h>

// This example illustrates how to implement the SAXPY
// operation (Y[i] = a * X[i] + Y[i]) using Thrust. 
// The saxpy_slow function demonstrates the most
// straightforward implementation using a temporary
// array and two separate transformations, one with
// multiplies and one with plus.  The saxpy_fast function
// implements the operation with a single transformation
// and represents "best practice".

using my_time_t = std::chrono::microseconds;

struct saxpy_functor : public thrust::binary_function<float,float,float> {
    private:
        const float a_;

    public:
        saxpy_functor(float a) : a_(a) {}

        __host__ __device__ float operator()(const float& x, const float& y) const { 
            return a_*x + y;
        }
};

void saxpy_fast(float a, thrust::device_vector<float>& X, thrust::device_vector<float>& Y) {
    // Y <- a*X + Y
    thrust::transform(X.begin(), X.end(), Y.begin(), Y.begin(), saxpy_functor(a));
}

void saxpy_slow(float a, thrust::device_vector<float>& X, thrust::device_vector<float>& Y) {
    thrust::device_vector<float> temp(X.size());
   
    // temp <- a
    thrust::fill(temp.begin(), temp.end(), a);
    
    // temp <- a*X
    thrust::transform(X.begin(), X.end(), temp.begin(), temp.begin(), thrust::multiplies<float>());

    // Y <- A*X + Y
    thrust::transform(temp.begin(), temp.end(), Y.begin(), Y.begin(), thrust::plus<float>());
}

int main(int argc, char* argv[]) {
    // handle command line arguments
    int size {10};
    if (argc > 1) {
        size = std::stoi(argv[1]);
    }

    // initialize host arrays
    std::vector<float> x;
    std::vector<float> y;
    for (int i = 0; i < size; ++i) {
        x.push_back(1.0f);
        y.push_back(i*1.0f);
    }

    {
        auto start_time = std::chrono::steady_clock::now();
        // transfer to device
        thrust::device_vector<float> X(x.begin(), x.end());
        thrust::device_vector<float> Y(y.begin(), y.end());

        // slow method
        saxpy_slow(2.0, X, Y);
        auto end_time = std::chrono::steady_clock::now();
        auto delta_time = std::chrono::duration_cast<my_time_t>(end_time - start_time);
        std::cout << "saxpy_slow took " << delta_time.count() << " us\n";
    }

    {
        auto start_time = std::chrono::steady_clock::now();
        // transfer to device
        thrust::device_vector<float> X(x.begin(), x.end());
        thrust::device_vector<float> Y(y.begin(), y.end());

        // fast method
        saxpy_fast(2.0, X, Y);
        auto end_time = std::chrono::steady_clock::now();
        auto delta_time = std::chrono::duration_cast<my_time_t>(end_time - start_time);
        std::cout << "saxpy_fast took " << delta_time.count() << " us\n";
    }
    
    {
        auto start_time = std::chrono::steady_clock::now();
        std::transform(x.begin(), x.end(), y.begin(), y.begin(),
               [] (float x, float y) { return 2.0f*x + y; });
        auto end_time = std::chrono::steady_clock::now();
        auto delta_time = std::chrono::duration_cast<my_time_t>(end_time - start_time);
        std::cout << "saxpy host took " << delta_time.count() << " us\n";

    }

    return 0;
}
