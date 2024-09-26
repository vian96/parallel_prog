#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int all = omp_get_max_threads();
    int counter = 0;
    omp_lock_t lock;
    omp_init_lock(&lock);

#pragma omp parallel
    {
        while (!omp_test_lock(&lock)) ;
        printf("thread %d, cntr is %d\n", omp_get_thread_num(), counter);
        counter++;
        omp_unset_lock(&lock);
    }
    printf("final counter is %d\n", counter);
    omp_destroy_lock(&lock);
}
