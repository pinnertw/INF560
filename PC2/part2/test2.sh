mpicc max1.c -o max2 #-D DEBUG
salloc -N 1 -n 1 mpirun ./max2 20 1000000000
