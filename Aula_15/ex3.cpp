#include <mpi.h>
#include <iostream>
#include <cstring>

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank (ID) do processo e o número total de processos
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Cria a mensagem
    const int tamanho_msg = 100;
    char mensagem[tamanho_msg];

    if (rank == 0) {
        // Processo 0 cria e envia a mensagem inicial
        strcpy(mensagem, "Mensagem iniciada no processo 0");
        MPI_Send(mensagem, strlen(mensagem) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo 0 enviou: \"" << mensagem << "\" para o processo 1" << std::endl;
    } 
    else {
        // Todos os outros processos recebem a mensagem do processo anterior
        MPI_Recv(mensagem, tamanho_msg, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: \"" << mensagem << "\" do processo " << rank - 1 << std::endl;

        // Se não for o último processo, enviar para o próximo
        if (rank < size - 1) {
            MPI_Send(mensagem, strlen(mensagem) + 1, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD);
            std::cout << "Processo " << rank << " enviou: \"" << mensagem << "\" para o processo " << rank + 1 << std::endl;
        }
    }

    // O último processo exibe a mensagem recebida
    if (rank == size - 1) {
        std::cout << "Processo " << rank << " (último) recebeu a mensagem final: \"" << mensagem << "\"" << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
