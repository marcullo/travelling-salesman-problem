#include <stdio.h>
#include <omp.h>

#include "memory.h"

int main() {
    printf("Hello, World at %.lf s!\n", omp_get_wtime());

    MEMORY_CHECK();
    return 0;
}
