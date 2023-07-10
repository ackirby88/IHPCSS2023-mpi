#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#define N 10 // Size of the grid

int main(int argc, char** argv) {
    int rank, size;
    int num_neighbors;
    int* neighbors;
    double grid[N][N];
    double grid_new[N][N];
    MPI_Comm graph_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Create the graph topology
    int* index = (int*)malloc(size * sizeof(int));
    int* edges = (int*)malloc(4 * size * sizeof(int));
    num_neighbors = 0;

    for (int i = 0; i < size; i++) {
        int x = i % (int)sqrt(size);
        int y = i / (int)sqrt(size);
        int x_minus = (x - 1 + (int)sqrt(size)) % (int)sqrt(size);
        int x_plus = (x + 1) % (int)sqrt(size);
        int y_minus = (y - 1 + (int)sqrt(size)) % (int)sqrt(size);
        int y_plus = (y + 1) % (int)sqrt(size);

        if (x_minus != x)
            edges[num_neighbors++] = x_minus + y * (int)sqrt(size);
        if (x_plus != x)
            edges[num_neighbors++] = x_plus + y * (int)sqrt(size);
        if (y_minus != y)
            edges[num_neighbors++] = x + y_minus * (int)sqrt(size);
        if (y_plus != y)
            edges[num_neighbors++] = x + y_plus * (int)sqrt(size);

        index[i] = num_neighbors;
    }

    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, 0, &graph_comm);
    free(index);
    free(edges);

    // Get the neighbors of the current process
    neighbors = (int*)malloc(num_neighbors * sizeof(int));
    MPI_Graph_neighbors(graph_comm, rank, num_neighbors, neighbors);

    // Initialize grid values
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            grid[i][j] = rank;
            grid_new[i][j] = 0.0;
        }
    }

    // Perform Jacobi iterations
    int iterations = 100;
    for (int iter = 0; iter < iterations; iter++) {
        // Exchange boundary values with neighbors
        MPI_Neighbor_alltoall(&grid[1][0], 1, MPI_DOUBLE, &grid[0][0], 1, MPI_DOUBLE, graph_comm);

        // Compute new values
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                grid_new[i][j] = 0.25 * (grid[i - 1][j] + grid[i + 1][j] + grid[i][j - 1] + grid[i][j + 1]);
            }
        }

        // Copy new values to the grid
        for (int i = 1; i < N - 1; i++) {
            for (int j = 1; j < N - 1; j++) {
                grid[i][j] = grid_new[i][j];
            }
        }
    }

    // Print the final grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", grid[i][j]);
        }
        printf("\n");
    }

    free(neighbors);
    MPI_Comm_free(&graph_comm);
    MPI_Finalize();

    return 0;
}
