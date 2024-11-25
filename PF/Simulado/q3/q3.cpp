#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <omp.h>
#include <algorithm> // Para std::min

void matrixMultiplyBlock(const std::vector<std::vector<float>>& A, 
                         const std::vector<std::vector<float>>& B, 
                         std::vector<std::vector<float>>& C, int N, int block_size) {
    // Multiplicação de matrizes em blocos com paralelismo
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int i = 0; i < N; i += block_size) {
        for (int j = 0; j < N; j += block_size) {
            for (int k = 0; k < N; k += block_size) {
                for (int ii = i; ii < std::min(i + block_size, N); ii++) {
                    for (int jj = j; jj < std::min(j + block_size, N); jj++) {
                        float temp = 0.0;
                        for (int kk = k; kk < std::min(k + block_size, N); kk++) {
                            temp += A[ii][kk] * B[kk][jj];
                        }
                        #pragma omp atomic
                        C[ii][jj] += temp;
                    }
                }
            }
        }
    }
}

int main() {
    // Tamanhos das matrizes para teste
    const std::vector<int> matrix_sizes = {500, 1000, 1500}; // Alterado de 2000 para 1500
    const int block_size = 64; // Tamanho ideal do bloco (ajustável para o cache da CPU)

    for (int N : matrix_sizes) {
        // Inicializa as matrizes
        std::vector<std::vector<float>> A(N, std::vector<float>(N));
        std::vector<std::vector<float>> B(N, std::vector<float>(N));
        std::vector<std::vector<float>> C(N, std::vector<float>(N, 0.0));

        // Atribui valores aleatórios às matrizes A e B
        srand(time(NULL));
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i][j] = static_cast<float>(rand() % 100);
                B[i][j] = static_cast<float>(rand() % 100);
            }
        }

        // Mede o tempo de execução
        double start_time = omp_get_wtime();
        matrixMultiplyBlock(A, B, C, N, block_size);
        double time_taken = omp_get_wtime() - start_time;

        // Imprime o resultado
        std::cout << "Tamanho da matriz: " << N << "x" << N << std::endl;
        std::cout << "Tempo de execução: " << time_taken << " segundos." << std::endl;
        std::cout << "--------------------------------------" << std::endl;
    }

    return 0;
}
