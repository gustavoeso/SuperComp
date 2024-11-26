#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>

const int MATRIX_SIZE = 4; // Tamanho fixo da matriz para o teste

// Inicializar a matriz com valores conhecidos para validar os cálculos
std::vector<int> initializeMatrix1D(int rows, int cols) {
    return {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
}

// Calcular as médias das linhas localmente usando OpenMP
std::vector<double> calculateRowAverages(const std::vector<int>& subMatrix, int rows, int cols) {
    std::vector<double> averages(rows, 0.0);

    #pragma omp parallel for
    for (int i = 0; i < rows; ++i) {
        double sum = 0.0;
        for (int j = 0; j < cols; ++j) {
            sum += subMatrix[i * cols + j];
        }
        averages[i] = sum / cols;
    }

    return averages;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int rowsPerProcess = MATRIX_SIZE / size; // Divisão das linhas entre os processos

    std::vector<int> matrix1D;
    if (rank == 0) {
        // Inicializar a matriz no formato 1D no processo raiz
        matrix1D = initializeMatrix1D(MATRIX_SIZE, MATRIX_SIZE);
    }

    // Cada processo recebe um subconjunto de linhas da matriz
    std::vector<int> subMatrix1D(rowsPerProcess * MATRIX_SIZE);
    MPI_Scatter(matrix1D.data(), rowsPerProcess * MATRIX_SIZE, MPI_INT,
                subMatrix1D.data(), rowsPerProcess * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular médias das linhas localmente
    std::vector<double> localAverages = calculateRowAverages(subMatrix1D, rowsPerProcess, MATRIX_SIZE);

    // Reunir todas as médias no processo raiz
    std::vector<double> globalAverages;
    if (rank == 0) {
        globalAverages.resize(MATRIX_SIZE);
    }
    MPI_Gather(localAverages.data(), rowsPerProcess, MPI_DOUBLE,
               globalAverages.data(), rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz calcula a média total
    if (rank == 0) {
        double totalSum = 0.0;
        for (const auto& avg : globalAverages) {
            totalSum += avg;
        }
        double overallAverage = totalSum / MATRIX_SIZE;
        std::cout << "Overall Average of All Rows: " << overallAverage << std::endl;
    }

    MPI_Finalize();
    return 0;
}
