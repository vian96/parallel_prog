#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <chrono>
using namespace std::chrono;

#define ISIZE 5000
#define JSIZE 5000

double a[ISIZE][JSIZE];

int main(int argc, char **argv) {
  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      a[i][j] = 10 * i + j;
    }
  }

  auto start = high_resolution_clock::now();
  for (i = 8; i < ISIZE; i++) {
    for (j = 0; j < JSIZE - 3; j++) {
      a[i][j] = sin(4 * a[i - 8][j + 3]);
    }
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  printf("time: %ld\n", duration.count());

  ff = fopen("result.txt", "w");
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      fprintf(ff, "%f ", a[i][j]);
    }
    fprintf(ff, "\n");
  }
  fclose(ff);
}
