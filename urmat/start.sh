mpicc main.c -lm -o main && mpirun -np $1 ./main $2 $3 && python3 plot.py $1
