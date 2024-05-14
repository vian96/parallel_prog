#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

double f(double x) {
    return sin(8*3.14*x);
}

double g(double t) {
    return sin(2*3.14*t);
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char file_name[1000] = {};
    sprintf(file_name, "out%d.csv", rank);
    FILE *file = fopen(file_name, "w");
    fprintf(file, "t,x,u\n");

    double a = 1.0; 
    double X = 1.0;  
    double T = 1.0;  
    int Nx = 1000;    
    int Nt = 2000;    
    if (argc > 2) {
        Nx = atoi(argv[1]);
        Nt = atoi(argv[2]);
    } 

    double dx = X / (Nx + 1);
    double dt = T / Nt;

    int local_Nx = Nx / size;

    // +2 bc ghost points
    double *u_old = calloc(local_Nx + 2,  sizeof(double));
    double *u_next = calloc(local_Nx + 2, sizeof(double));

    for (int i = 0; i <= local_Nx; i++)
        u_old[i] = f( (rank*local_Nx + i) * dx);

    for (int n = 0; n < Nt; n++) {
        // will probably break if synchronous?
        if (rank < size-1) 
            MPI_Send(&u_old[local_Nx], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);

        if (rank > 0) 
            MPI_Recv(&u_old[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        else // rank == 0
            u_old[0] = g(dt*n);

        for (int i = 1; i <= local_Nx; ++i) {
            if (i == local_Nx) // does not it lose accuracy?
                u_next[i] = u_old[i] - a * dt / dx * (u_old[i] - u_old[i - 1]);
            else    
                u_next[i] = - (u_old[i+1] - u_old[i-1]) / 2 / dx * dt + 0.5 * (u_old[i+1] + u_old[i-1]);
        }

        for (int i = 1; i <= local_Nx; i++) {
            u_old[i] = u_next[i];
            fprintf(file, "%d,%d,%f\n", n, rank*local_Nx + i, u_old[i]);
        }
    }

    MPI_Finalize();
}
