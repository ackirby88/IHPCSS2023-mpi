#!/bin/bash
mpirun -np 8 ./mpi_stencil_graph 512 10 10000 4 2
