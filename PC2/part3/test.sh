mpicc max4.c -o max3 #-D DEBUG
echo "1000 samples"
salloc -N 1 -n 5 mpirun ./max3 20 1000 2
echo "10000 samples"
salloc -N 1 -n 5 mpirun ./max3 20 10000 2
