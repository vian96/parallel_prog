#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <omp.h>

#include <chrono>
using namespace std::chrono;

#define ISIZE 5000
#define JSIZE 5000

double ans[ISIZE][JSIZE];
double res[ISIZE][JSIZE];

int main(int argc, char **argv) {
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      ans[i][j] = 10 * i + j;
      res[i][j] = 10 * i + j;
    }
  }

  int cnt[ISIZE-4] = {};
  auto start = high_resolution_clock::now();

#pragma omp parallel for collapse(2)
  for (i = 0; i < ISIZE - 4; i++) {
    //int cnt = 0;
    for (j = 5; j < JSIZE; j++) {
      //printf("thread %d %d\n", omp_get_thread_num(), i);
      res[i][j] = sin(0.1 * ans[i + 4][j - 5]);
    }
    //   printf("%d: %d\n", i, cnt[i]);
  }

  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  fprintf(stderr, "par time: %ld\n", duration.count());

    ff = fopen("2result_mt.txt", "w");
   // printf("p=%p\n", ff);
    //printf("%d opened\n", rank);
    for (i = 0; i < ISIZE; i++) {
      //fprintf(stderr, "%d %d %d\n", rank, i, ISIZE);
      for (j = 0; j < JSIZE; j++) {
     //   printf("%d %d %d\n", rank, i, j);
        fprintf(ff, "%f ", res[i][j]);
        //printf("%f ", res[i][j]);
      }
      fprintf(ff, "\n");
      //printf("\n");
    }
    fclose(ff);
}
