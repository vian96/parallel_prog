#include <stdio.h>
#include <omp.h>

int a = 0;
int b = 0;

int main() {
    #pragma omp threadprivate(a)
    #pragma omp threadprivate(b)

    a = omp_get_thread_num();

    #pragma omp parallel copyin(a)
        printf("Thread %d: a = %d\n", omp_get_thread_num(), a);

    #pragma omp parallel
    {
        b = omp_get_thread_num();
        #pragma omp single copyprivate(b)
            printf("Thread %d: b = %d (after copyprivate)\n", omp_get_thread_num(), b);
        printf("Thread %d: b = %d\n", omp_get_thread_num(), b);
    }
}

