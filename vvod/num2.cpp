#include <omp.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
    int all = omp_get_max_threads();
    if (argc < 2) {
        printf("not enough args!\n");
        return 1;
    }
    int n = atoi(argv[1]);
    if (n < 1) {
        printf("not number arg!\n");
        return 1;
    }
    //    int q = (n + all - 1) / all; // points per thread // not used
    float *arr = (float*)calloc(all, sizeof(float));

#pragma omp parallel for 
    for (int i=n; i>0; i--) {
//        printf("%d\n", omp_get_thread_num());
        arr[(size_t)omp_get_thread_num()] += 1./i;
    }

    printf("calced\n");

    for (int i=all-2; i>=0; i--) {
        printf("%f\n", arr[i]);
        arr[all-1] += arr[i];
    }
    printf("%f\n", arr[all-1]);

    free(arr);
}
