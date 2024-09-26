#include <omp.h>
#include <stdio.h>

int main() {
    printf("OMP: %d REAL: 8thr/4cores\n", omp_get_max_threads());
}
