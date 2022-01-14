#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

// Number of columns in a matrix
#define N 16

MPI_Status status;
double matrix_a[N][N],matrix_b[N][N];
double begin, end;

int main(int argc, char **argv)
{
  int processCount, processId, slaveTaskCount, source, dest, columns, offset, temp;
  struct timeval start, stop;
  MPI_Init(&argc, &argv);   // MPI initialization 
  MPI_Comm_rank(MPI_COMM_WORLD, &processId);   // ID (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &processCount); // Number of processes 


   MPI_Barrier(MPI_COMM_WORLD);
   begin = MPI_Wtime();

  slaveTaskCount = processCount - 1;   // Number of slave tasks 

 if (processId == 0) {
    srand ( time(NULL) );
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        matrix_a[i][j]= ((double) i) * ((double) j);
      }
    }

    printf("\n Matrix A[i][j]\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
        printf("%.0f\t", matrix_a[i][j]);
      }
            printf("\n");
    }

// Determine number of columns of the Matrix A, which is sent to each slave process
    columns = N/slaveTaskCount;
    temp = columns;

// Offset variable - starting point 
    offset = 0;

// Each message's tag = 1
    for (dest=1; dest <= slaveTaskCount; dest++)
    {
      columns = temp * dest;
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&columns, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      MPI_Send(&matrix_a[0][offset], columns*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      offset = offset + columns;
    }
	
    for (int i = 1; i <= slaveTaskCount; i++)
    {
      source = i;
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&columns, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      MPI_Recv(&matrix_b[0][offset], columns*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

// Print the result matrix
    printf("\n[>] SQRT Matrix :\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++)
        printf("%.2f\t", matrix_b[i][j]);
      printf ("\n");
    }
	sleep(3);
    printf ("\n");

  }

// Slave Processes
  if (processId > 0) {
    source = 0;
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&columns, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    MPI_Recv(&matrix_a, columns*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
	
    // sqrt Matrix 
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
         matrix_b[i][j] = pow(matrix_a[i][j],0.5);

        }
    }

    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&columns, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    MPI_Send(&matrix_b, columns*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);

  }

  // End time calculation
  MPI_Barrier(MPI_COMM_WORLD);
  end = MPI_Wtime();

  printf("\n");
  printf("\nDuration : %f [PID : %d]\n", end-begin-3, processId);
  MPI_Finalize();
}
