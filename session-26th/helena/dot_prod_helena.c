#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char** args){
    int rank, size, sendcount, C, sendC, *A, *B, *recvbufA, *recvbufB; 

    //MPI setup
    MPI_Init(&argc, &args);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    //Declare the Matrices

    if(rank == 0){
       A = malloc(sizeof(int)* 100000);
       B = malloc(sizeof(int)* 100000);
       C = 0;

        for(int i=0 ; i<100000; i++){
            A[i] = 1; 
            B[i] = 5;
        }
    }

        //Define the needed Arguments and Buffers
        sendcount = 100000 / size;

        recvbufA = malloc(sizeof(int) * sendcount);
        recvbufB = malloc(sizeof(int) * sendcount); 
        sendC = 0;

        //Send the Arrays
        MPI_Scatter(A, sendcount, MPI_INT, recvbufA, sendcount, MPI_INT, 0, MPI_COMM_WORLD);

        MPI_Scatter(B, sendcount, MPI_INT, recvbufB, sendcount, MPI_INT, 0, MPI_COMM_WORLD);
        

        //Calculate the Dotproduct
       for(int i = 0 ; i < sendcount; i++){

         sendC += recvbufA[i] * recvbufB[i];

        }

        //Calculate the Sum of the Data provided by the processes
        MPI_Reduce(&sendC, &C, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);


        free(recvbufA);
        free(recvbufB);

    if(rank == 0){
        //Print out the Data to check
        printf("The Result is %d! \n", C);  
        
        free(A);
        free(B);
        
    }

    //Close application
    MPI_Finalize();
    return 0; 

}
