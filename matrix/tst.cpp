#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>
#include <chrono>

#define BLOCK_SIZE 16

using namespace std::chrono;

void matmul(const std::vector<std::vector<double>>& A,
            const std::vector<std::vector<double>>& B,
            std::vector<std::vector<double>>& C) {
    int n = A.size();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            for (int k = 0; k < n; k += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE && ii < n; ++ii) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < n; ++jj) {
                        for (int kk = k; kk < k + BLOCK_SIZE && kk < n; ++kk) {
                            C[ii][jj] += A[ii][kk] * B[jj][kk];
                        }
                    }
                }
            }
        }
    }
}

void matmul_un(const std::vector<std::vector<double>>& A,
            const std::vector<std::vector<double>>& B,
            std::vector<std::vector<double>>& C) {
    int n = A.size();

    #pragma omp parallel for collapse(2)
    for (int ii = 0; ii < n; ++ii) {
        for (int jj = 0; jj < n; ++jj) {
            for (int kk = 0; kk < n; ++kk) {
                C[ii][jj] += A[ii][kk] * B[jj][kk];
            }
        }
    }
}

int main() {
    //std::ofstream out("stats_mt.csv");
  //  out << "n,t,type\n";
    for (int n = 32; n<=2048; n *= 4) {
        std::cout << n << std::endl;
        std::vector<std::vector<double>> A(n, std::vector<double>(n, 1.0));
        std::vector<std::vector<double>> B(n, std::vector<double>(n, 1.0));
        std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));
        std::vector<std::vector<double>> D(n, std::vector<double>(n, 0.0));

        auto begin = std::chrono::high_resolution_clock::now();
        matmul(A, B, C);
        auto end = std::chrono::high_resolution_clock::now();
//        out << n << ',' << duration_cast<microseconds>(end-begin).count() << ",block\n";

        begin = std::chrono::high_resolution_clock::now();
        matmul_un(A, B, D);
        bool equal = true;
        for (int i =0; i < n; i++)
                for (int j=0; j < n; j++)
                    if (C[i][j] != D[i][j]) {
                    equal = false;
                    std::cout << "wrong value\n";
                    return 0;
                }
        end = std::chrono::high_resolution_clock::now();
  //      out << n << ',' << duration_cast<microseconds>(end-begin).count() << ",simple\n";


        std::cout << "Matrix multiplication completed!" << std::endl;
    }
    return 0;
}

