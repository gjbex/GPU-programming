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
        fprintf(stderr, "malloc failed\n");
        return 1;
    }
#pragma omp target enter data map(to:a[0:n*n])
#pragma omp target teams distribute parallel for
    for (int i = 0; i < n; i++) {
        a[i*n + i] = 0.0f;
    } 
    float *b = (float *) omp_target_alloc(n*n*sizeof(float), omp_get_default_device());
    if (!b) {
        fprintf(stderr, "omp_target_alloc failed\n");
        return 1;
    }
#pragma omp target teams distribute parallel for is_device_ptr(b)
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            b[i*n + j] = ((float) (i*n + j))/(n*n);
        }
    }
    for (int iter = 0; iter < nr_iters; iter++) {
#pragma omp target teams distribute parallel for is_device_ptr(b)
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                a[i*n + j] += b[i*n + j];
            }
        }
    }
    omp_target_free(b, omp_get_default_device());
#pragma omp target exit data map(from:a[0:n*n])
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
