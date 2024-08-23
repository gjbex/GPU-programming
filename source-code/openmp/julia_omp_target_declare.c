#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

void init_z(float complex *z, int n);
void iterate_z(const float complex *z, int *count, int n,
               const float complex C);

#pragma omp declare target to(init_z, iterate_z)

void print_count(const int *count, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      printf("%4d", count[i * n + j]);
    }
    printf("\n");
  }
}

int main(int argc, char *argv[]) {
  int n = argc > 1 ? atoi(argv[1]) : 100;
  float complex C =
      argc > 2 ? atof(argv[2]) + I * atof(argv[3]) : -0.8f + I * 0.156f;
  float complex *z = (float complex *)malloc(n * n * sizeof(float complex));
  if (z == NULL) {
    fprintf(stderr, "malloc failed for %d elements\n", n * n);
    return 1;
  }
  int *count;
  if ((count = (int *)malloc(n * n * sizeof(int))) == NULL) {
    fprintf(stderr, "malloc failed for %d elements\n", n * n);
    free(z);
    return 1;
  }
#pragma omp target map(tofrom : count [0:n * n]) map(alloc : z [0:n * n])
  {
    init_z(z, n);
    iterate_z(z, count, n, C);
  }
  print_count(count, n);
  free(count);
  free(z);
  return 0;
}

void init_z(float complex *z, int n) {
  const float min = -1.8f, max = 1.8f;
  const float delta = (max - min) / n;
#pragma omp target teams distribute parallel for
  for (int i = 0; i < n; i++) {
    double re = min + i * delta;
    for (int j = 0; j < n; j++) {
      double im = min + j * delta;
      z[i * n + j] = re + I * im;
    }
  }
}

void iterate_z(const float complex *z, int *count, int n,
               const float complex C) {
#pragma omp target teams distribute parallel for
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      float complex Z = z[i * n + j];
      count[i * n + j] = 0;
      while (creal(Z) * creal(Z) + cimag(Z) * cimag(Z) <= 4.0f &&
             count[i * n + j]++ < 255) {
        Z = Z * Z + C;
      }
    }
  }
}
