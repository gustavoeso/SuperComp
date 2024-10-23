#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank (ID) do processo e o número total de processos
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cada processo tem um valor exclusivo (pode ser definido de forma dinâmica, mas aqui é apenas o rank)
    int meu_valor = rank + 1;  // Valor exclusivo de cada processo (aqui usando o rank para facilitar)
    int soma_parcial = 0;

    if (rank == 0) {
        // Processo 0 inicia com seu próprio valor
        soma_parcial = meu_valor;
        std::cout << "Processo 0 iniciou com valor: " << meu_valor << std::endl;

        // Envia a soma parcial para o processo 1
        MPI_Send(&soma_parcial, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } 
    else {
        // Todos os outros processos recebem o valor parcial do processo anterior
        MPI_Recv(&soma_parcial, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        // Soma seu próprio valor
        soma_parcial += meu_valor;
        std::cout << "Processo " << rank << " recebeu soma parcial: " << soma_parcial - meu_valor 
                  << " e somou seu valor " << meu_valor << " para obter: " << soma_parcial << std::endl;

        // Se não for o último processo, envia a soma parcial para o próximo
        if (rank < size - 1) {
            MPI_Send(&soma_parcial, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        } else {
            // O último processo exibe a soma total
            std::cout << "Processo " << rank << " (último) exibe a soma total: " << soma_parcial << std::endl;
        }
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
