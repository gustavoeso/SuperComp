#include <mpi.h>
#include <omp.h>
#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    const int N = 100000; // Substitua pelo tamanho desejado do vetor
    std::vector<int> vetor;
    std::vector<int> subvetor;

    if (world_rank == 0) {
        // Inicializa o vetor no processo raiz
        vetor.resize(N);
        for (int i = 0; i < N; ++i) {
            vetor[i] = i + 1;
        }
    }

    // Calcula os tamanhos dos subvetores
    int base_size = N / world_size; // Tamanho base de cada subvetor
    int extra = N % world_size;    // Elementos extras para os primeiros 'extra' processos

    int local_size = base_size + (world_rank < extra ? 1 : 0); // Ajusta o tamanho do subvetor
    subvetor.resize(local_size);

    // Define os deslocamentos e distribui os subvetores
    std::vector<int> send_counts(world_size);
    std::vector<int> displs(world_size);
    if (world_rank == 0) {
        int offset = 0;
        for (int i = 0; i < world_size; ++i) {
            send_counts[i] = base_size + (i < extra ? 1 : 0);
            displs[i] = offset;
            offset += send_counts[i];
        }
    }

    MPI_Scatterv(vetor.data(), send_counts.data(), displs.data(), MPI_INT, 
                 subvetor.data(), local_size, MPI_INT, 
                 0, MPI_COMM_WORLD);

    // Calcula a soma dos quadrados usando OpenMP
    long long soma_parcial = 0;
    #pragma omp parallel for reduction(+:soma_parcial)
    for (int i = 0; i < local_size; ++i) {
        soma_parcial += static_cast<long long>(subvetor[i]) * subvetor[i];
    }

    // Reduz os resultados para o processo raiz
    long long soma_total = 0;
    MPI_Reduce(&soma_parcial, &soma_total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        auto end_time = std::chrono::high_resolution_clock::now();
        std::cout << "Soma total dos quadrados: " << soma_total << std::endl;
    }

    MPI_Finalize();
    return 0;
}
