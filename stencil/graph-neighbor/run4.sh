#!/bin/bash
mpirun -np 4 ./mpi_stencil_graph 512 10 10000 2 2
