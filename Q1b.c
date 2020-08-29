/*
 * Alyxandra Spikerman
 * High Perfomance Computing
 * Homework 5 - Question 1b
 *
 * each node bins 1 class
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

    // we're working with a data range of 1000, so to get the class size, we need to divide the range by
    // the number of classes
    int DATA_RANGE = 1000;
    double CLASS_SIZE = DATA_RANGE / (double) CLASSES;

    srand(150);
    int values[N];
    for (int i = 0; i < N; i++) {
        values[i] = (rand() % DATA_RANGE) + 1;
    }

    MPI_Init(&argc, &argv); // being parallel code
    MPI_Comm_size(MPI_COMM_WORLD, &num_processors);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        printf("N = %d\nClasses= %d\n", N, CLASSES);
        printf("class size: %d\n", CLASS_SIZE);
        printf("num processors = %d\n", num_processors);
    }

    // initialize local and final histogram
    int final_histogram[CLASSES];
    int local_histogram[CLASSES];
    for (int i = 0; i < CLASSES; i++){
        final_histogram[i] = 0;
        local_histogram[i] = 0;
    }

    // each processor 'owns' a class, so get their class by looping through and addingthe num_processors
    // on each iteration
    for (int i = rank; i < CLASSES; i += num_processors) {
        for (int j = 0; j < N; j++) {
            if (((int)ceil(values[j] / CLASS_SIZE) - 1) == i) {
                local_histogram[i]++;
            }
        }
    }

    for (int i = 0; i < CLASSES; i++) {
        // using MPI_Reduce
        MPI_Reduce(&local_histogram[i], &final_histogram[i], 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        printf("\nTotal histogram values for %d classes\n", CLASSES);
        for (int i = 0; i < CLASSES; i++) {
            printf("Class %d: %d \n", i, final_histogram[i]);
        }
    }
    MPI_Finalize(); // end parallel code

    return 0;
}
