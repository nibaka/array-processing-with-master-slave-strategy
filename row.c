#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>

#define N 16

MPI_Status status;


double matrix_a[N][N],matrix_b[N][N];

double begin=0.0, end=0.0;

int main(int argc, char **argv)
{
  int processCount, processId, slaveTaskCount, source, dest, rows, offset;

  struct timeval start, stop;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &processId);

  MPI_Comm_size(MPI_COMM_WORLD, &processCount);

  MPI_Barrier(MPI_COMM_WORLD);
  begin = MPI_Wtime();

  slaveTaskCount = processCount - 1;

// Master
 if (processId == 0) {

    srand ( time(NULL) );
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        matrix_a[i][j]= ((double) i) * ((double) j);
      }
    }

  printf("\n\t\t                  Matrix A[i][j]                    \n" );

// Print Matrix A
    printf("\n[>] Matrix A\n\n" );
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
            printf("\n");
    }

    rows = N/slaveTaskCount;
    offset = 0;

    for (dest=1; dest <= slaveTaskCount; dest++)
    {
      
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      offset = offset + rows;
    }

    for (int i = 1; i <= slaveTaskCount; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_b[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

// Print the result matrix
    printf("\nSQRT MATRIX :\n\n" );
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++)
        printf("%.2f\t", matrix_b[i][j]);
      printf ("\n");
    }
    printf ("\n");

  }

// Slave Processes
  if (processId > 0) {

    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);


    // Matrix sqrt

    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        matrix_b[i][j] = pow(matrix_a[i][j],0.5);

        }
    }

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&matrix_b, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  // End time calculation
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();

  printf("Duration : %f --> [PID : %d]\n", end-begin, processId);
  MPI_Finalize();
}