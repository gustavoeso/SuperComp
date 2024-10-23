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

    const int tamanho_msg = 100;
    char mensagem_enviar[tamanho_msg];
    char mensagem_receber[tamanho_msg];

    if (rank == 0) {
        // Processo 0 envia uma mensagem e recebe resposta de todos os processos
        for (int i = 1; i < size; i++) {
            // Enviar a primeira mensagem para o processo i
            sprintf(mensagem_enviar, "Mensagem do processo 0 para o processo %d", i);
            MPI_Send(mensagem_enviar, strlen(mensagem_enviar) + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou: \"" << mensagem_enviar << "\" para o processo " << i << std::endl;

            // Receber a resposta do processo i
            MPI_Recv(mensagem_receber, tamanho_msg, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::cout << "Processo 0 recebeu: \"" << mensagem_receber << "\" do processo " << i << std::endl;
        }
    } 
    else {
        // Processos 1 até size-1 recebem e respondem ao processo 0
        MPI_Recv(mensagem_receber, tamanho_msg, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: \"" << mensagem_receber << "\" do processo 0" << std::endl;

        // Responde ao processo 0
        sprintf(mensagem_enviar, "Resposta do processo %d para o processo 0", rank);
        MPI_Send(mensagem_enviar, strlen(mensagem_enviar) + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: \"" << mensagem_enviar << "\" para o processo 0" << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
