#include <armadillo>
#include <iostream>

int main(int argc, char** argv) {
  const int nr_elements{argc > 1 ? std::stoi(argv[1]) : 10};
  const int nr_qps{argc > 2 ? std::stoi(argv[2]) : 15};
  const int vector_size{argc > 3 ? std::stoi(argv[3]) : 5};
  arma::cube A(nr_elements, nr_qps, vector_size);
  A.fill(1.0);
  arma::mat B(nr_elements, vector_size);
  B.fill(2.0);
  arma::mat C(nr_elements, nr_qps);
  for (int element = 0; element < nr_elements; ++element) {
    for (int qp = 0; qp < nr_qps; ++qp) {
      double total{0.0};
      for (int i = 0; i < vector_size; ++i) {
        total += A(element, qp, i) * B(element, i);
      }
      C(element, qp) = total;
    }
  }
  double sum{0.0};
  for (int element = 0; element < nr_elements; ++element) {
    for (int qp = 0; qp < nr_qps; ++qp) {
      sum += C(element, qp);
    }
  }
  std::cout << "Sum: " << sum << std::endl;
  return 0;
}
