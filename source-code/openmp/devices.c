#include <stdio.h>
#include <omp.h>

int main() {
    printf("%d devices detected\n", omp_get_num_devices());
    return 0;
}
