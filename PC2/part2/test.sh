mpicc max2.c -o max2 #-D DEBUG
salloc -N 1 -n 5 mpirun ./max2 20 1000000000
