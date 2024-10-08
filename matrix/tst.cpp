#include <iostream>
#include <vector>

#define BLOCK_SIZE 16

void matmul(const std::vector<std::vector<double>>& A,
            const std::vector<std::vector<double>>& B,
            std::vector<std::vector<double>>& C) {
    int n = A.size();

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

    for (int ii = 0; ii < n; ++ii) {
        for (int jj = 0; jj < n; ++jj) {
            for (int kk = 0; kk < n; ++kk) {
                C[ii][jj] += A[ii][kk] * B[kk][jj];
            }
        }
    }
}

int main() {
    int n = 2048; // Matrix size (nxn)
    std::vector<std::vector<double>> A(n, std::vector<double>(n, 1.0));
    std::vector<std::vector<double>> B(n, std::vector<double>(n, 1.0));
    std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));

    matmul_un(A, B, C);

    std::cout << "Matrix multiplication completed!" << std::endl;
    return 0;
}

