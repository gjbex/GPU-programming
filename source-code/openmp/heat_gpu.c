#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char *argv[]) {
    const int n = argc > 1 ? atoi(argv[1]) : 1000;
    const int nr_iters = argc > 2 ? atoi(argv[2]) : 100;

    float *temp = malloc(n*n*sizeof(float));
    if (temp == NULL) {
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
    float *temp_new = malloc(n*n*sizeof(float));
    if (temp_new == NULL) {
        fprintf(stderr, "malloc failed\n");
        free(temp);
        return 1;
    }
#pragma omp target enter data map(to:temp[0:n*n]) map(to:temp_new[0:n*n])
#pragma omp target teams distribute parallel for
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            temp[i*n + j] = 0.0f;
        }
    }
#pragma omp target teams distribute parallel for
    for (int i = 0; i < n; i++) {
        temp[i*n] = temp_new[i*n] = 1.0f;
        temp[i*n + n - 1] = temp_new[i*n + n - 1] = 1.0f;
    }
#pragma omp target teams distribute parallel for
    for (int j = 0; j < n; j++) {
        temp[j] = temp_new[j] = 1.0f;
        temp[(n - 1)*n + j] = temp_new[(n - 1)*n] = 1.0f;
    }
    for (int iter = 0; iter < nr_iters; iter++) {
#pragma omp target teams distribute parallel for
        for (int i = 1; i < n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                temp_new[i*n + j] = 0.25f*(temp[i*n + j - 1] + temp[i*n + j + 1] + temp[(i - 1)*n + j] + temp[(i + 1)*n + j]);
            }
        }
#pragma omp target teams distribute parallel for
        for (int i = 1; i < n - 1; i++) {
            for (int j = 1; j < n - 1; j++) {
                temp[i*n + j] = temp_new[i*n + j];
            }
        }
        if (iter % 10 == 0) {
            float sum = 0.0f;
#pragma omp target update from(temp[0:n*n])
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    sum += temp[i*n + j];
                }
            }
            printf("iter = %d, sum = %f\n", iter, sum);
        }
    }
#pragma omp target exit data map(from:temp[0:n*n])
    free(temp_new);
    free(temp);
    return 0;
}
