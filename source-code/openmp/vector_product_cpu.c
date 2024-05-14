#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NX 2048000

int main()
{
    float *vecA, *vecB, *vecC;
    vecA = (float *) malloc(NX*sizeof(float));
    vecB = (float *) malloc(NX*sizeof(float));
    vecC = (float *) malloc(NX*sizeof(float));
    if (vecA == NULL || vecB == NULL || vecC == NULL) {
        fprintf(stderr, "error: can not allocated array of size %d\n", NX);
        return 1;
    }

    float r = 0.2f;

    /* Initialization of vectors */
    for (int i = 0; i < NX; i++) {
        vecA[i] = pow(r, i);
        vecB[i] = 1.0f;
        vecC[i] = 0.0f;
    }

    /* dot product of two vectors */
#pragma omp parallel for
    for (int i = 0; i < NX; i++) {
        for (int j = 0; j < 10000; j++) {
            vecC[i] += vecA[i] * vecB[i];
        }
    }

    float sum = 0.0;
    /* calculate the sum */
    for (int i = 0; i < NX; i++) {
        sum += vecC[i];
    }
    printf("The sum is: %8.6f \n", sum);
    free(vecA);
    free(vecB);
    free(vecC);

    return 0;
}
