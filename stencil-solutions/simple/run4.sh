#!/bin/bash
mpirun -np 4 ./mpi_stencil 512 10 10000 2 2
