#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  const int n = argc > 1 ? atoi(argv[1]) : 10000;
  const int N = argc > 2 ? atoi(argv[2]) : 2048000;
  float *vecA, *vecB, *vecC;
  vecA = (float *)malloc(N * sizeof(float));
  vecB = (float *)malloc(N * sizeof(float));
  vecC = (float *)malloc(N * sizeof(float));
  if (vecA == NULL || vecB == NULL || vecC == NULL) {
    fprintf(stderr, "error: can not allocated array of size %d\n", N);
    return 1;
  }

  const float r = 0.2f;
  for (int i = 0; i < N; i++) {
    vecA[i] = r * i;
    vecB[i] = 2.1f;
    vecC[i] = 0.0f;
  }

#pragma omp target teams parallel for map(to               \
                                          : vecA[:N], vecB \
                                          [:N]) map(tofrom \
                                                    : vecC[:N])
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < n; j++) {
      vecC[i] += vecA[i] * vecB[i];
    }
  }

  float sum = 0.0f;
  for (int i = 0; i < N; i++) {
    sum += vecC[i];
  }
  printf("The sum is: %8.6e \n", sum);

  free(vecA);
  free(vecB);
  free(vecC);

  return 0;
}
