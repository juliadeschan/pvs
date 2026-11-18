// Source - https://stackoverflow.com/q/45781405
// Posted by Bran M. Junior, modified by community. See post 'Timeline' for
// change history Retrieved 2026-11-18, License - CC BY-SA 3.0

#include "mpi.h"
#include <stdio.h>
#include <string.h>

// this is a dot product
int main(int argc, char *argv[]) {
  int my_rank, p;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);

  int A[100000];
  int B[100000];
  int C = 0;

  for (int i = 0; i < 100000; i++) {
    C += A[i] * B[i];
  }
}
