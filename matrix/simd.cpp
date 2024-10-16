#include <iostream>
#include <vector>
#include <omp.h>
#include <fstream>
#include <chrono>
#include <immintrin.h>
#include <algorithm>

#define BLOCK_SIZE 16

using namespace std::chrono;

void matmul(const std::vector<std::vector<double>>& A,
            const std::vector<std::vector<double>>& B,
            std::vector<std::vector<double>>& C) {
    // all matrices' dimensions are assumed to be divisible by 4
    int n = A.size();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            for (int k = 0; k < n; k += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE && ii < n; ++ii) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < n; jj+=4) {
                        // C[i]_j = sum a[i][k] * b[k]_j;
                        __m256d c0 = _mm256_loadu_pd(&C[ii][jj]);
                        for (int kk = k; kk < k + BLOCK_SIZE && kk < n; ++kk) {
                            __m256d a0 = _mm256_set1_pd(A[ii][kk]);
                            __m256d b0 = _mm256_loadu_pd(&B[kk][jj]);
                            c0 = _mm256_fmadd_pd(a0, b0, c0); // c0 += a0 * b0
                        }
                        _mm256_storeu_pd(&C[ii][jj], c0);
                    }
                }
            }
        }
    }
}

inline double hsum_double_avx(__m256d v) { // from https://stackoverflow.com/questions/49941645/get-sum-of-values-stored-in-m256d-with-sse-avx
    __m128d vlow  = _mm256_castpd256_pd128(v);
    __m128d vhigh = _mm256_extractf128_pd(v, 1); // high 128
            vlow  = _mm_add_pd(vlow, vhigh);     // reduce down to 128
    __m128d high64 = _mm_unpackhi_pd(vlow, vlow);
    return  _mm_cvtsd_f64(_mm_add_sd(vlow, high64));  // reduce to scalar
}


void matmul3(const std::vector<std::vector<double>>& A,
            const std::vector<std::vector<double>>& B,
            std::vector<std::vector<double>>& C) {
    // all matrices' dimensions are assumed to be divisible by 4
    int n = A.size();

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < n; i += BLOCK_SIZE) {
        for (int j = 0; j < n; j += BLOCK_SIZE) {
            for (int k = 0; k < n; k += BLOCK_SIZE) {
                for (int ii = i; ii < i + BLOCK_SIZE && ii < n; ++ii) {
                    for (int jj = j; jj < j + BLOCK_SIZE && jj < n; ++jj) {
                        __m256d c0 = _mm256_setzero_pd();
                        for (int kk = k; kk < k + BLOCK_SIZE && kk < n; kk+=4) {
                            __m256d a0 = _mm256_loadu_pd(&A[ii][kk]);
                            __m256d b0 = _mm256_loadu_pd(&B[jj][kk]);
                            c0 = _mm256_fmadd_pd(a0, b0, c0); // c0 += a0 * b0
                        }
                        C[ii][jj] += hsum_double_avx(c0);
                    }
                }
            }
        }
    }
}

void matmul4(const std::vector<std::vector<double>>& A,
            const std::vector<std::vector<double>>& B,
            std::vector<std::vector<double>>& C) {
    // all matrices' dimensions are assumed to be divisible by 4
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

int check_res(const std::vector<std::vector<double>> &C, const std::vector<std::vector<double>> &D) {
    int n = C.size();
    for (int i =0; i < n; i++)
        for (int j=0; j < n; j++)
            if (C[i][j] != D[i][j]) {
                std::cout << "wrong value\n";
                return 1;
            }
    return 0;
}

void make_v_0(std::vector<std::vector<double>> &C) {
    for (auto &i:C)
        std::fill(i.begin(), i.end(), 0);
}

int main() {
    std::ofstream out("csv_simd.csv");
    out << "n,t,type\n";
    for (int n = 32; n<=2048; n *= 4) {
        std::cout << n << std::endl;
        std::vector<std::vector<double>> A(n, std::vector<double>(n, 1.0));
        std::vector<std::vector<double>> B(n, std::vector<double>(n, 1.0));
        std::vector<std::vector<double>> C(n, std::vector<double>(n, 0.0));
        std::vector<std::vector<double>> D(n, std::vector<double>(n, 0.0));

        matmul4(A,B,C); // for cache + validation
        make_v_0(D);

        auto begin = std::chrono::high_resolution_clock::now();
        matmul4(A, B, D);
        auto end = std::chrono::high_resolution_clock::now();
        out << n << ',' << duration_cast<microseconds>(end-begin).count() << ",block\n";
        if (check_res(C,D))
            return 1;
        std::cout << "block is successful\n";
        make_v_0(D);

        begin = std::chrono::high_resolution_clock::now();
        matmul(A, B, D);
        end = std::chrono::high_resolution_clock::now();
        out << n << ',' << duration_cast<microseconds>(end-begin).count() << ",simd1\n";
        if (check_res(C,D))
            return 1;
        std::cout << "simd is successful\n";
        make_v_0(D);

        begin = std::chrono::high_resolution_clock::now();
        matmul3(A, B, D);
        end = std::chrono::high_resolution_clock::now();
        out << n << ',' << duration_cast<microseconds>(end-begin).count() << ",simd3\n";
        if (check_res(C,D))
            return 1;
        std::cout << "simd 3 is successful\n";
        make_v_0(D);


        std::cout << "Matrix multiplication completed!" << std::endl;
    }
    return 0;
}

