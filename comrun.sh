src=${1:-1}
g++ -fopenmp num${src}.cpp && ./a.out $2
