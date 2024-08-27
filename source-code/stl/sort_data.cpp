#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

int main(int argc, char *argv[]) {
  const int size{argc > 1 ? std::stoi(argv[1]) : 1000};
  std::vector<int> data;
  std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> dis(0.0, 1.0);
  for (int i = 0; i < size; i++) {
    data.push_back(dis(gen));
  }
  auto start = std::chrono::high_resolution_clock::now();
  std::sort(std::execution::par, data.begin(), data.end());
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "Time to sort " << size << " elements: " << diff.count() << " s"
            << std::endl;
  return 0;
}
