#include <mpi.h>
#include "stdio.h"
// Source - https://stackoverflow.com/q/45781405
// Posted by Bran M. Junior, modified by community. See post 'Timeline' for change history
// Retrieved 2026-11-18, License - CC BY-SA 3.0

// mpicc zum kompilieren
// mpirun -np X, um das Programm auf X Prozessen zu starten

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, p;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);   // Abfrage der Prozessnummer
    MPI_Comm_size(MPI_COMM_WORLD, &p);      // Abfrage der Gesamtanzahl der Prozesse

    printf("rank %d of %d", rank, p);

    MPI_Finalize();

    return 0;
}