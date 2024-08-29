#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

int random_walk(int nr_steps, std::mt19937 &gen) {
  std::uniform_int_distribution<int> dis(0, 1);
  int pos{0};
  for (int i = 0; i < nr_steps; i++) {
    pos += 2 * dis(gen) - 1;
  }
  return pos;
}

int main(int argc, char *argv[]) {
  const int nr_walks{argc > 1 ? std::stoi(argv[1]) : 1000};
  const int nr_steps{argc > 2 ? std::stoi(argv[2]) : 1000};
  const int seed{1234};
  std::mt19937 gen(seed);
  std::vector<int> positions(2 * nr_steps + 1, 0);
  for (int i = 0; i < nr_walks; i++) {
    int pos{random_walk(nr_steps, gen)};
    positions[pos + nr_steps]++;
  }
  for (int i = 0; i < 2 * nr_steps + 1; i++) {
    std::cout << i - nr_steps << " " << positions[i] << std::endl;
  }
  return 0;
}
