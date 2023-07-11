#!/bin/bash
#==================================================================
#See: https://web.cels.anl.gov/~thakur/sc16-mpi-tutorial/slides.pdf
#Code Repository: https://web.cels.anl.gov/~thakur/sc16-mpi-tutorial/code-examples.zip
#
#Advanced MPI Programming Tutorial
#Tutorial at SC16, November 2016
#Pavan Balaji, William Gropp, Torsten Hoefler, Rajeev Thakur
#==================================================================
#To compile the programs, run "make".

#You can run any of the "stencil" programs as follows:
mpiexec -n 8 ./stencil_mpi_carttopo_neighcolls 512 10 1000 4 2

