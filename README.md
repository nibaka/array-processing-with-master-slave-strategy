# array-processing-with-master-slave-strategy
Array Processing includes the square root of each element in rows and columns with MPI

Serial_code:
It navigates the array once in a row and once in a column and calculates the amount of time.

MPI:
The operation performed on each of the elements of this array is the calculation of the square root of each element.
For example, the initial value of A63 is 18 and the square root of it is 24.4
Using the salve-Master method, parallel code that the master initializes the array and then distributes the array among the slaves. 
The slaves then perform the calculations and the data is finally collected by the master to generate the final output.


