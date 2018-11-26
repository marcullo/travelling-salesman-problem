#include <stdio.h>
#include <omp.h>

int main() {
    printf("Hello, World at %.lf s!\n", omp_get_wtime());
    return 0;
}
