mpicc hello.c -o hello
salloc -n 4 -N 2 mpirun hello
