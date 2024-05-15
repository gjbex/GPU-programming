#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void iterate_z(int *count, const int n, const float complex C, const int max_iters) {
    const float min = -1.8f, max = 1.8f;
    const float delta = (max - min)/n;
#pragma omp parallel for default(none) shared(count, n, min, delta, C, max_iters)
    for (int i = 0; i < n; i++) {
        double re = min + i*delta;
        for (int j = 0; j < n; j++) {
            double im = min + j*delta;
            float complex Z = re + I*im;
            count[i*n + j] = 0;
            while (creal(Z)*creal(Z) + cimag(Z)*cimag(Z) <= 4.0f &&
                    count[i*n + j]++ < max_iters) {
                Z = Z*Z + C;
            }
        }
    }
}

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
    int *count;
    if ((count = (int *) malloc(n*n*sizeof(int))) == NULL) {
        fprintf(stderr, "malloc failed for %d elements\n", n*n);
        return 1;
    }
    iterate_z(count, n, C, max_iters);
    print_count(count, n);
    free(count);
    return 0;
}
