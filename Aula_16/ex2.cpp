#include <mpi.h>
#include <iostream>
#include <cmath>

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_iteracoes;

    if (rank == 0) {
        // Processo raiz define a configuração inicial (número de iterações)
        num_iteracoes = 100000;  // Exemplo de número de iterações
        std::cout << "Processo raiz definindo número de iterações para: " << num_iteracoes << std::endl;
    }

    // Distribui a configuração para todos os processos
    MPI_Bcast(&num_iteracoes, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo executa um cálculo intensivo baseado no número de iterações
    double resultado_local = 0.0;
    for (int i = 0; i < num_iteracoes; ++i) {
        resultado_local += std::sin(i * 0.001);  // Cálculo intensivo de exemplo
    }

    // Cada processo imprime seu resultado local
    std::cout << "Processo " << rank << " completou o cálculo com resultado: " << resultado_local << std::endl;

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
