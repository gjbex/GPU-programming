#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char *argv[]) {
    const int n = argc > 1 ? atoi(argv[1]) : 1000;
    const int nr_iters = argc > 2 ? atoi(argv[2]) : 10;
    float *a = (float *) malloc(n*n*sizeof(float));
    if (!a) {
        fprintf(stderr, "Error: could not allocate memory\n");
        return 1;
    }
#pragma omp target data map(tofrom:a[0:n*n])
    {
#pragma omp target teams distribute parallel for
        for (int i = 0; i < n; i++) {
            a[i*n + i] = 0.0f;
        } 
        float *b = (float *) malloc(n*n*sizeof(float));
        if (!b) {
            fprintf(stderr, "Error: could not allocate memory\n");
            return 1;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                b[i*n + j] = 2.0f*(rand()/(float) RAND_MAX - 0.5f);
            }
        }
#pragma omp target data map(to:b[0:n*n])
        {
            for (int iter = 0; iter < nr_iters; iter++) {
#pragma omp target teams distribute parallel for
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        a[i*n + j] += b[i*n + j];
                    }
                }
            }
        }
        free(b);
    }
    float sum = 0.0f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            sum += a[i*n + j];
        }
    }
    printf("sum = %f\n", sum);
    free(a);
    return 0;
}
