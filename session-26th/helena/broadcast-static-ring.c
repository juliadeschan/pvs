#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include <string.h>

int main(int argv, char** args){

    int rank, size; 
    int tag = 0; 

    char* msg = malloc(sizeof(char)*30);
    MPI_Status status;

    MPI_Init(&argv, &args);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    if(rank == 0){
        strcpy(msg, "This is a broadcast.");
        MPI_Send(msg, 30, MPI_CHAR, 1, tag, MPI_COMM_WORLD);
    }

    MPI_Recv(msg, 30, MPI_CHAR, (rank - 1), tag, MPI_COMM_WORLD, &status);

    printf("%s Process %d \n", msg, rank);


    if(rank == (size - 1)){

        MPI_Send(msg, 30, MPI_CHAR, 0, tag, MPI_COMM_WORLD);

    }else{

        MPI_Send(msg, 30, MPI_CHAR, (rank + 1), tag, MPI_COMM_WORLD);

    }

    free(msg);


    MPI_Finalize();	
    return 0;
}
