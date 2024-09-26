#include <iostream>
#include <omp.h>
#include <vector>
#include <utility>
#include <stdlib.h>

unsigned long my_rand() {
    static unsigned seed = 4654;
    seed = (1103515245 * seed + 12345) % (1ULL<<31);
    return seed%(1<<20);
}

int main() {
    std::vector<std::vector<int>> hist(4);
//    int *hist = (int*)calloc(65*4, sizeof(int));
    #pragma omp parallel for schedule(SCHEDULER, CHUNK_SIZE) num_threads(4)
    for (int i=0; i<65; i++) {
        hist[omp_get_thread_num()].push_back(i);
//        hist[65*omp_get_thread_num() + i] = i;
        int sum=0;
        int up = my_rand(); // not thread safe but who cares
        for (int k=0; k < up; k++)
            sum += k;
    }

    for (int i=0; i < 4; i++) {
        std::cout << i << ' ';
        for (const auto &n:hist[i])
            std::cout << n << ' ';
        //        for (int n=0; n<65;n++)
  //          std::cout << hist[i*65+n] << ' ';
        std::cout << std::endl;
    }
}
