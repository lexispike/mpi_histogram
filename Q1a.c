/*
 * Alyxandra Spikerman
 * High Perfomance Computing
 * Homework 5 - Question 1a
 *
 * each node bins for x classes
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, num_processors;

    int N, CLASSES;
    if (argc == 3) {
        N = atoi(argv[1]);
        CLASSES = atoi(argv[2]);
    } else {
        printf("Error: must input 2 arguments, <N> <# of classes>\n");
        return 1;
    }

    MPI_Init(&argc, &argv); // being parallel code
    MPI_Comm_size(MPI_COMM_WORLD, &num_processors); // get the total # of processors
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get this threads rank

    // we're working with a data range of 1000, so to get the class size, we need to divide the range by
    // the number of classes
    int DATA_RANGE = 1000;
    double CLASS_SIZE = DATA_RANGE / (double) CLASSES;

    // initialize the local and final histogram
    int local_histogram[CLASSES], final_histogram[CLASSES];
    for (int i = 0; i < CLASSES; i++){
        final_histogram[i] = 0;
        local_histogram[i] = 0;
    }

    // each thread will check N in chunks
    int chunk = N / num_processors;
    int val = 0;
    srand(rank + 10);
    if (rank == 0) {
        printf("N = %d\nClasses= %d\n", N, CLASSES);
        printf("class size: %d\n", CLASS_SIZE);
        printf("chunk: %d\n", chunk);
    }

    for (int i = 0; i < chunk; i++) {
        val = (rand() % DATA_RANGE) + 1; // get the value to check
        local_histogram[(int)ceil(val / CLASS_SIZE) - 1]++; // count the value in one of the classes
    }

    for (int i = 0; i < CLASSES; i++) {
        // using MPI_Reduce
        MPI_Reduce(&local_histogram[i], &final_histogram[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("Total histogram values for %d classes\n", CLASSES);
        for (int i = 0; i < CLASSES; i++) {
            printf("Class %d: %d \n", i, final_histogram[i]);
        }
    }
    MPI_Finalize(); // end parallel code

    return 0;
}
