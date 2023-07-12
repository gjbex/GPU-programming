#include <algorithm>
#include <chrono>
#include <cmath>
#include <complex>
#include <execution>
#include <iostream>
#include <valarray>

using namespace std;
using NanoTime = chrono::nanoseconds;

using Complex = complex<float>;

valarray<float> coordinates(float min_coord, float max_coord,
                             size_t steps);
valarray<Complex> z_values(const valarray<float>& x_coords,
                         const valarray<float>& y_coords);
valarray<int> iterate_zs(valarray<Complex>& zs, const Complex& c,
                         size_t max_iters);
void print_results(const valarray<int>& ns);

int main(int argc, char *argv[]) {
    const Complex c(-0.62772f, - 0.42193f);
    const float x1 {-1.8f};
    const float x2 {1.8f};
    const float y1 {-1.8f};
    const float y2 {1.8f};
    const size_t max_iters {255};
    size_t steps {100};
    if (argc > 1)
        steps = stoi(argv[1]);
    valarray<float> x_coords = coordinates(x1, x2, steps);
    valarray<float> y_coords = coordinates(y1, y2, steps);
    valarray<Complex> zs = z_values(x_coords, y_coords);
    auto start_time = chrono::steady_clock::now();
    valarray<int> ns = iterate_zs(zs, c, max_iters);
    auto end_time = chrono::steady_clock::now();
    auto duration = chrono::duration_cast<NanoTime>(end_time - start_time);
    cerr << "time: " << duration.count()*1.0e-9 << " s" << endl;
    print_results(ns);
    return 0;
}

valarray<float> coordinates(float min_coord, float max_coord,
                             size_t steps) {
    valarray<float> coords(steps);
    const float step {(max_coord - min_coord)/steps};
    float value {min_coord};
    for (size_t i = 0; i < steps; i++) {
        coords[i] = value;
        value += step;
    }
    return coords;
}

valarray<Complex> z_values(const valarray<float>& x_coords,
                         const valarray<float>& y_coords) {
    valarray<Complex> zs(x_coords.size()*y_coords.size());
    size_t i {0};
    for (auto y: y_coords)
        for (auto x: x_coords) {
            Complex z(x, y);
            zs[i++] = z;
        }
    return zs;
}

int iterate_z(Complex& z, const Complex& c, size_t max_iters) {
    size_t n {0};
    while (real(z)*real(z) + imag(z)*imag(z) < 4.0f && n++ < max_iters)
        z = z*z + c;
    return n;
}

valarray<int> iterate_zs(valarray<Complex>& zs, const complex<float>& c,
                         size_t max_iters) {
    valarray<int> ns(zs.size());
    std::transform(std::execution::seq,
            std::begin(zs), std::end(zs), std::begin(ns),
            [=] (Complex& z) { return iterate_z(z, c, max_iters); });
    return ns;
}

void print_results(const valarray<int>& ns) {
    size_t steps = (static_cast<size_t>(sqrt(ns.size()) + 0.1f));
    size_t count {0};
    for (auto n: ns) {
        cout << n;
        if (++count % steps == 0)
            cout << endl;
        else
            cout << " ";
    }
}
