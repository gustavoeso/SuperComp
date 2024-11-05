#include <iostream>
#include <mpi.h>
#include <vector>
#include <ctime>
#include <cstdlib>

// Função para inicializar a matriz com valores aleatórios
void initializeMatrix(std::vector<std::vector<int>>& matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = rand() % 10;  // Valores aleatórios de 0 a 9
        }
    }
}

// Função para imprimir a matriz (para depuração)
void printMatrix(const std::vector<std::vector<int>>& matrix, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 4;  // Tamanho da matriz (N x N)
    std::vector<std::vector<int>> matrix(N, std::vector<int>(N));
    std::vector<int> flat_matrix(N * N);  // Matriz unidimensional para facilitar o scatter
    int chunk_size = (N * N) / size;  // Quantidade de elementos para cada processo

    if (rank == 0) {
        // Inicializa a matriz no processo 0
        srand(time(0));
        initializeMatrix(matrix, N);
        std::cout << "Matriz gerada no processo 0:" << std::endl;
        printMatrix(matrix, N);

        // Converter a matriz 2D para 1D
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                flat_matrix[i * N + j] = matrix[i][j];
            }
        }
    }

    // Vetor local para cada processo armazenar seus elementos
    std::vector<int> local_data(chunk_size);

    // Distribuição da matriz para cada processo
    MPI_Scatter(flat_matrix.data(), chunk_size, MPI_INT, local_data.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcula a soma local
    int local_sum = 0;
    for (int i = 0; i < chunk_size; i++) {
        local_sum += local_data[i];
    }

    // Reduz a soma local em uma soma global no processo 0
    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Calcula a média dividindo a soma total pelo número total de elementos
        double media = static_cast<double>(global_sum) / (N * N);
        std::cout << "Soma total da matriz: " << global_sum << std::endl;
        std::cout << "Média dos elementos da matriz: " << media << std::endl;
    }

    MPI_Finalize();
    return 0;
}
