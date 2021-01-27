gcc hello_world.c -o hello -fopenmp
OMP_NUM_THREADS=4 salloc -n 1 ./hello
