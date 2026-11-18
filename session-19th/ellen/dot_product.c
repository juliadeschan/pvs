#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "mpi.h" // steuerung + shift + p → Edit Configuration (UI) → /usr/bin/mpicc


int main(int argc, char *argv[])
{
    int rank, size, root = 0;
    float local_sum, dotproduct;
    int vector_1[] = {2, 5, 7, 8, 4, 2, 6, 9};
    int vector_2[] = {4, 8, 3, 6, 7, 2, 5, 1};
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int len_1 = sizeof(vector_1)/(sizeof(vector_1[0]));
    int len_2 = sizeof(vector_2)/(sizeof(vector_2[0]));

    if(len_1 != len_2) {
        printf("Failure: Vectors not of the same size!!");
        return 1;
    }    

    int count = len_1/ size;        // supposing the length of the vector is dividable by number of processes

    // each process determines the dot product of its own part

    for (int i = 0 ; i < count; i++) {
        local_sum += vector_1[count*rank + i] * vector_2[count * rank + i];
    }

    // in the root process we some up all the part solutions:
    MPI_Reduce(&local_sum, &dotproduct, count, MPI_FLOAT, MPI_SUM, root, MPI_COMM_WORLD);

    if(rank == root) {
        printf("The dot product is: %.2f", dotproduct);
    }

    MPI_Finalize();
    return 0;
}