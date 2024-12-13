mpic++ ${1}par.cpp -lm -o main${1} && mpirun -np $2 ./main${1} # && python3 plot.py $1
