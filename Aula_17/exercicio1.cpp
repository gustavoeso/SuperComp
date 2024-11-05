#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <chrono>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 1000; // Tamanho da matriz (pode aumentar para testar desempenho)
    std::vector<int> data(N * N);

    // Inicialização da matriz apenas no processo 0
    if (rank == 0) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i * N + j] = i + j;
            }
        }
    }

    // Dividir a matriz entre os processos MPI
    int chunk_size = (N * N) / size;
    std::vector<int> local_data(chunk_size);

    MPI_Scatter(data.data(), chunk_size, MPI_INT, local_data.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Medir o tempo
    auto start = std::chrono::high_resolution_clock::now();

    // Cálculo do quadrado de cada elemento (com OpenMP)
    #pragma omp parallel for
    for (int i = 0; i < chunk_size; ++i) {
        local_data[i] *= local_data[i];
    }

    // Reunir os resultados no processo 0
    MPI_Gather(local_data.data(), chunk_size, MPI_INT, data.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Medir o tempo final
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    if (rank == 0) {
        std::cout << "Tempo de execução (com OpenMP): " << elapsed.count() << " segundos" << std::endl;
    }

    // Repetir o cálculo sem OpenMP para comparação
    if (rank == 0) {
        for (int i = 0; i < N * N; ++i) {
            data[i] = i / N + i % N; // Reinicializar a matriz para o cálculo sem OpenMP
        }
    }

    MPI_Scatter(data.data(), chunk_size, MPI_INT, local_data.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    start = std::chrono::high_resolution_clock::now();

    // Cálculo do quadrado de cada elemento (sem OpenMP)
    for (int i = 0; i < chunk_size; ++i) {
        local_data[i] *= local_data[i];
    }

    MPI_Gather(local_data.data(), chunk_size, MPI_INT, data.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;

    if (rank == 0) {
        std::cout << "Tempo de execução (sem OpenMP): " << elapsed.count() << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
