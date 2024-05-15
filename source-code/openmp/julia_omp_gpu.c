#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

void print_count(const int *count, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", count[i*n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    const int n = argc > 1 ? atoi(argv[1]) : 100;
    const int max_iters = argc > 2 ? atoi(argv[2]) : 255;
    const float complex C = argc > 3 ? atof(argv[3]) + I*atof(argv[4]) : -0.8f + I*0.156f;
    const int dev_nr = omp_get_default_device();
    fprintf(stderr, "Using device %d\n", dev_nr);
    int *count;
    if ((count = (int *) malloc(n*n*sizeof(int))) == NULL) {
        fprintf(stderr, "malloc failed for %d elements\n", n*n);
        return 1;
    }
    const float min = -1.8f, max = 1.8f;
    const float delta = (max - min)/n;
#pragma omp target teams distribute parallel for map(tofrom: count[:n*n])
    for (int i = 0; i < n; i++) {
        const double re = min + i*delta;
        for (int j = 0; j < n; j++) {
            const double im = min + j*delta;
            const int idx = i*n + j;
            float complex Z = re + I*im;
            count[idx] = 0;
            while (creal(Z)*creal(Z) + cimag(Z)*cimag(Z) <= 4.0f &&
                    count[idx]++ < max_iters) {
                Z = Z*Z + C;
            }
        }
    }
    print_count(count, n);
    free(count);
    return 0;
}
