#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


int main(int argc, char** args){
    int rank, size, sendcount; 

    //MPI setup
    MPI_Init(&argc, &args);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);


    //Declare the Matrices

    int* A = malloc(sizeof(int)* 10000000);
    int* B = malloc(sizeof(int)* 10000000);
    
    int C = 0;

    if(rank == 0){
        
        for(int i=0 ; i<10000000; i++){
            A[i] = 1; 
            B[i] = 5;
        }
    }
    
    //Anscheinend muss hier gelten : Arraylength = sendcount * size; wenn das nicht gilt dann was?
    if (10000000 % size == 0 ){

        //Define the needed Arguments and Buffers
        sendcount = 10000000 / size;

        int* recvbufA = malloc(sizeof(int) * sendcount);
        int* recvbufB = malloc(sizeof(int) * sendcount); 
        int sendC = 0;


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

    }else{
        //Obv doesnt work in this case, how to fix? 
        MPI_Finalize();
        return 0;
    }

    if(rank == 0){
        //Print out the Data to check
        printf("The Result is %d! \n", C);    
    }

    free(A);
    free(B);

    //Close application
    MPI_Finalize();
    return 0; 

}