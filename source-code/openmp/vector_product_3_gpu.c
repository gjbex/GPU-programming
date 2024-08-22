#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char *argv[]) {
    const int n = argc > 1 ? atoi(argv[1]) : 10000;
    const int N = argc > 2 ? atoi(argv[2]) : 2048000;
    float *vecA = (float *) omp_target_alloc(N*sizeof(float), omp_get_default_device());
    float *vecB = (float *) omp_target_alloc(N*sizeof(float), omp_get_default_device());
    float *vecC = (float *) omp_target_alloc(N*sizeof(float), omp_get_default_device());
    if (vecA == NULL || vecB == NULL || vecC == NULL) {
        fprintf(stderr, "error: can not allocated array of size %d\n", N);
        return 1;
    }

    float sum = 0.0f;

    const float r = 0.2f;
#pragma omp target teams loop is_device_ptr(vecA, vecB, vecC)
    for (int i = 0; i < N; i++) {
        vecA[i] = r*i;
        vecB[i] = 2.1f;
        vecC[i] = 0.0f;
    }

#pragma omp target teams loop is_device_ptr(vecA, vecB, vecC)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < n; j++) {
            vecC[i] += vecA[i]*vecB[i];
        }
    }

#pragma omp target teams loop reduction(+:sum) is_device_ptr(vecA, vecB, vecC)
    for (int i = 0; i < N; i++) {
        sum += vecC[i];
    }
    printf("The sum is: %8.6e \n", sum);

    omp_target_free(vecA, omp_get_default_device());
    omp_target_free(vecB, omp_get_default_device());
    omp_target_free(vecC, omp_get_default_device());

    return 0;
}
