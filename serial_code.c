#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <unistd.h>

//#define begin clock()
//#define end clock()
#define N 16
double matrix_a[N][N],matrix_b[N][N];
clock_t begin ;
clock_t end ;
//double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

int main(int argc, char **argv)
{
	
	//______________________________________________________________row______________________________________________________________________
	
	clock_t begin =clock();
	printf("\n Matrix A[i][j]\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++) {
          matrix_a[i][j]=((double) i * (double)j );
        printf("%.0f\t", matrix_a[i][j]);
      }
            printf("\n");
    }
    
    printf("\n[>] SQRT Matrix :\n\n");
    for (int i = 0; i<N; i++) {
      for (int j = 0; j<N; j++){
	    matrix_b[i][j] = sqrt(matrix_a[i][j]);
        printf("%.2f\t", matrix_b[i][j]);
      }
      printf ("\n");
    }
    
    printf ("\n");
	
    clock_t end  =clock() ;
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\duration : %f" ,time_spent);

	
	//_____________________________column_______________________
    
	clock_t begin1 =clock();
    printf("\n[>] Matrix A\n\n" );
	
    for (int j = 0; j<N; j++) {
      for (int i = 0; i<N; i++) {
		matrix_a[i][j]=((double) i * (double)j );
        printf("%.0f\t", matrix_a[i][j]);
      }
            printf("\n");
    }
	
    printf("\n[>] SQRT Matrix :\n\n" );
    for (int j = 0; j<N; j++) {
      for (int i = 0; i<N; i++){
	    matrix_b[i][j] = sqrt(matrix_a[i][j]);
        printf("%.2f\t", matrix_b[i][j]);
      }
      printf ("\n");
    }
	sleep(3);
    printf ("\n");
	

clock_t end1  =clock() ;
	
double time_spent1 = (double)(end1 - begin1) / CLOCKS_PER_SEC;
printf("\duration : %f" ,time_spent1);
  }
  

  


  