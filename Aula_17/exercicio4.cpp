#include <iostream>
#include <mpi.h>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int N = 100;  // Tamanho total do vetor
    const int target = 5;  // Valor a ser procurado
    std::vector<int> vector;

    if (rank == 0) {
        // Inicializa o vetor no processo 0 com valores aleatórios entre 0 e 9
        vector.resize(N);
        srand(time(0));
        for (int i = 0; i < N; ++i) {
            vector[i] = rand() % 10;
        }

        std::cout << "Vetor gerado no processo 0: ";
        for (int i = 0; i < N; ++i) {
            std::cout << vector[i] << " ";
        }
        std::cout << std::endl;
    }

    // Distribui o vetor para todos os processos
    int chunk_size = N / size;
    std::vector<int> local_vector(chunk_size);
    MPI_Scatter(vector.data(), chunk_size, MPI_INT, local_vector.data(), chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Vetor para armazenar as posições locais encontradas
    std::vector<int> local_positions;

    // Busca o valor-alvo no subvetor local usando OpenMP
    #pragma omp parallel for
    for (int i = 0; i < chunk_size; ++i) {
        if (local_vector[i] == target) {
            #pragma omp critical
            local_positions.push_back(i + rank * chunk_size);  // Ajusta o índice global
        }
    }

    // Gathers sizes of positions from each process
    int local_size = local_positions.size();
    std::vector<int> all_sizes(size);
    MPI_Gather(&local_size, 1, MPI_INT, all_sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Gathers all positions in the root process
    std::vector<int> displs(size, 0);
    int total_positions = 0;
    if (rank == 0) {
        for (int i = 1; i < size; ++i) {
            displs[i] = displs[i - 1] + all_sizes[i - 1];
        }
        total_positions = displs[size - 1] + all_sizes[size - 1];
    }

    std::vector<int> global_positions(total_positions);
    MPI_Gatherv(local_positions.data(), local_size, MPI_INT, global_positions.data(), all_sizes.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    // Exibe as posições encontradas no processo 0
    if (rank == 0) {
        std::cout << "Posições do valor " << target << " encontradas em: ";
        for (int pos : global_positions) {
            std::cout << pos << " ";
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
