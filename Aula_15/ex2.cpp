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

    // Verifica se o número de processos é maior que 2
    if (size <= 2) {
        if (rank == 0) {
            std::cerr << "Este programa requer mais de 2 processos." << std::endl;
        }
        MPI_Finalize();
        return 0;
    }

    // Cria a mensagem a ser enviada
    const int tamanho_msg = 100;
    char mensagem[tamanho_msg];

    if (rank == 0) {
        // Processo 0 envia a mensagem inicial
        strcpy(mensagem, "Mensagem do processo 0");
        MPI_Send(mensagem, strlen(mensagem) + 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        std::cout << "Processo 0 enviou: " << mensagem << " para o processo 1" << std::endl;

        // Processo 0 recebe a mensagem de volta do último processo
        MPI_Recv(mensagem, tamanho_msg, MPI_CHAR, size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo 0 recebeu de volta: " << mensagem << " do processo " << size - 1 << std::endl;
    } 
    else {
        // Todos os outros processos recebem e enviam a mensagem
        MPI_Recv(mensagem, tamanho_msg, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: " << mensagem << " do processo " << rank - 1 << std::endl;

        // Enviar a mensagem para o próximo processo, ou de volta para o processo 0 se for o último
        int destino = (rank + 1) % size;
        MPI_Send(mensagem, strlen(mensagem) + 1, MPI_CHAR, destino, 0, MPI_COMM_WORLD);
        std::cout << "Processo " << rank << " enviou: " << mensagem << " para o processo " << destino << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
