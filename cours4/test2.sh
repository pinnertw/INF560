gcc test2.c -o prog2 -fopenmp
OMP_NUM_THREADS=4 salloc -n 1 ./prog2
