#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h>

void print_array(float *x, int N) {
    for (int i = 0; i < N; i++) {
        printf("%7.3f", x[i]);
    }
    printf("\n");
}

__global__ void saxpy(float *y,
        const float a, const float *x, const long int N) {
    long int i = blockDim.x*blockIdx.x + threadIdx.x;

    if (i < N) {
        y[i] += a*x[i];
    }
}

int main(int argc, char *argv[]) {
    cudaError_t err = cudaSuccess;
    const int block_size = 32;
    const float alpha = 1.3f;
    int array_size = block_size; 
    if (argc > 1) {
        array_size = atoi(argv[1]);
    }
    float *x, *y;
    if (!(x = (float *) malloc(array_size*sizeof(float)))) {
        fprintf(stderr, "error: can not allocate x\n");
        exit(1);
    }
    if (!(y = (float *) malloc(array_size*sizeof(float)))) {
        fprintf(stderr, "error: can not allocate y\n");
        exit(1);
    }
    for (int i = 0; i < array_size; i++) {
        x[i] = i*0.01f;
        y[i] = 0.5f;
    }
    if (array_size < 100) {
        print_array(y, array_size);
        printf("%7.3f\n", alpha);
        print_array(x, array_size);
    }
    float *x_dev, *y_dev;
    err = cudaMalloc(&x_dev, array_size*sizeof(float));
    if (err != cudaSuccess) {
        fprintf(stderr, "error: could not allocate device array x\n");
        exit(2);
    }
    err = cudaMemcpy(x_dev, x, array_size*sizeof(float), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        fprintf(stderr, "error: could not copy to device array x\n");
        exit(2);
    }
    cudaMalloc(&y_dev, array_size*sizeof(float));
    if (err != cudaSuccess) {
        fprintf(stderr, "error: could not allocate device array y\n");
        exit(2);
    }
    err = cudaMemcpy(y_dev, y, array_size*sizeof(float), cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        fprintf(stderr, "error: could not copy to device array y\n");
        exit(2);
    }
    saxpy<<<(array_size + block_size)/block_size, block_size>>>(y_dev, alpha, x_dev, array_size);
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        fprintf(stderr, "error: could not compute saxpy: %s\n", cudaGetErrorName(err));
        exit(3);
    }
    err = cudaMemcpy(y, y_dev, array_size*sizeof(float), cudaMemcpyDeviceToHost);
    if (err != cudaSuccess) {
        fprintf(stderr, "error: could not copy from device array y\n");
        exit(2);
    }
    if (array_size < 100) {
        print_array(y, array_size);
    }
    float max_error = 0.0f;
    for (int i = 0; i < array_size; i++) {
        float diff = fabs(y[i] - (0.5f + alpha*i*0.01f));
        if (diff > max_error) {
            max_error = diff;
        }
    }
    printf("max. error = %.7f\n", max_error);
    cudaFree(x_dev);
    cudaFree(y_dev);
    free(x);
    free(y);
    return 0;
}

