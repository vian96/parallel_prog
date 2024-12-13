#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <chrono>
using namespace std::chrono;

#define ISIZE 5000
#define JSIZE 5000

double a[ISIZE][JSIZE];
double b[ISIZE][JSIZE];

int main(int argc, char **argv) {
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = 10 * i + j;
      b[i][j] = 0;
    }
  }

  auto start = high_resolution_clock::now();
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = sin(0.001 * a[i][j]);
    }
  }
  for (i = 0; i < ISIZE - 3; i++) {
    for (j = 5; j < JSIZE; j++) {
      b[i][j] = a[i + 3][j - 5] * 3;
    }
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  printf("time: %ld\n", duration.count());

//  ff = fopen("3result.txt", "w");
//  for (i = 0; i < ISIZE; i++) {
//    for (j = 0; j < JSIZE; j++) {
//      fprintf(ff, "%f ", b[i][j]);
//    }
//    fprintf(ff, "\n");
//  }
//  fclose(ff);
}
