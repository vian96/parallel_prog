#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    int all = omp_get_max_threads();
    int counter = 0;

#pragma omp parallel for ordered
    for (int i=0; i<all; i++) {
        #pragma omp ordered
        {
        printf("thread %d, cntr is %d\n", omp_get_thread_num(), counter);
        counter++;
        }
    }
    printf("final counter is %d\n", counter);
}
