gcc pi_omp.c -o pi -fopenmp
OMP_NUM_THREADS=6 salloc -n 1 ./pi 100000 0
