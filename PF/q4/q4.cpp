#include <iostream>
#include <vector>
#include <mpi.h>
#include <omp.h>
#include <random>
#include <iomanip> // Para formatar a saída

const int MATRIX_SIZE = 1000; // Ajuste o tamanho conforme necessário

// Função para inicializar uma matriz como um vetor 1D para facilitar a distribuição por processos
std::vector<int> initializeMatrix1D(int rows, int cols) {
    std::vector<int> matrix(rows * cols);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = dis(gen);
    }
    return matrix;
}

// Função para calcular as médias das linhas localmente usando OpenMP
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
        std::cout << "Processo 0: Matriz inicializada." << std::endl;
    }

    // Sincronização para organizar a saída
    MPI_Barrier(MPI_COMM_WORLD);

    // Cada processo recebe um subconjunto de linhas da matriz
    std::vector<int> subMatrix1D(rowsPerProcess * MATRIX_SIZE);
    MPI_Scatter(matrix1D.data(), rowsPerProcess * MATRIX_SIZE, MPI_INT,
                subMatrix1D.data(), rowsPerProcess * MATRIX_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Mensagem para indicar que cada processo recebeu seus dados
    #pragma omp critical
    {
        std::cout << "Processo " << rank << " recebeu " << rowsPerProcess << " linhas." << std::endl;
    }

    // Calcular médias das linhas localmente
    std::vector<double> localAverages = calculateRowAverages(subMatrix1D, rowsPerProcess, MATRIX_SIZE);

    // Mostrar as médias locais calculadas
    #pragma omp critical
    {
        std::cout << "Processo " << rank << " calculou as seguintes médias locais: ";
        for (const auto& avg : localAverages) {
            std::cout << std::fixed << std::setprecision(2) << avg << " ";
        }
        std::cout << std::endl;
    }

    // Reunir todas as médias no processo raiz
    std::vector<double> globalAverages;
    if (rank == 0) {
        globalAverages.resize(MATRIX_SIZE);
    }
    MPI_Gather(localAverages.data(), rowsPerProcess, MPI_DOUBLE,
               globalAverages.data(), rowsPerProcess, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz calcula a média total
    if (rank == 0) {
        std::cout << "Processo 0: Recebeu médias de todos os processos." << std::endl;

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
