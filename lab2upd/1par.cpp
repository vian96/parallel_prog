#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#include <chrono>
using namespace std::chrono;

#define ISIZE 5000
#define JSIZE 5000

double ans[ISIZE][JSIZE];
double res[ISIZE][JSIZE];

int main(int argc, char **argv) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  fprintf(stderr, "r: %d s: %d\n", rank, size);

  if (size != 8) {
    fprintf(stderr, "i only work with size of 8:(\n");
    return 1;
  }

  int i, j;
  FILE *ff;
  for (i = 0; i < ISIZE; i++) {
    for (j = 0; j < JSIZE; j++) {
      ans[i][j] = 10 * i + j;
      res[i][j] = 10 * i + j;
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  auto start = high_resolution_clock::now();
  for (i = 8 + rank; i < ISIZE; i+=8) {
    for (j = 0; j < JSIZE - 3; j++) {
      ans[i][j] = sin(4 * ans[i - 8][j + 3]);
    }
  }
  //fprintf(stderr, "%d fin\n", rank);
  //MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
//    printf("0 sts\n");
    for (int i=8 + rank; i < ISIZE; i+=8) {
 //     printf("0: %d\n", i);
        MPI_Gather(ans[i], JSIZE, MPI_DOUBLE,
                   res[i], JSIZE, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
       // MPI_Barrier(MPI_COMM_WORLD);
      //fprintf(stderr, "0 e: %d\n", i);
    }
 //   printf("0 fins\n");
  } else {
   // printf("%d sts\n", rank);
    for (int i=8 + rank; i < ISIZE; i+=8) {
        MPI_Gather(ans[i], JSIZE, MPI_DOUBLE,
                   nullptr, 0, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
        //MPI_Barrier(MPI_COMM_WORLD);
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  fprintf(stderr, "r:%d, time: %ld\n", rank, duration.count());

  if (rank == 0) {
 //   printf("%d trying\n", rank);
    ff = fopen("result_mt.txt", "w");
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
  MPI_Finalize();
}
