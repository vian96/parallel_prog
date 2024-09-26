src=${1:-1}
g++ -fopenmp ${src} && ./a.out $2
