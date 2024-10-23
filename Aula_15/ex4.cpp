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

    if (rank == 0) {
        // Processo 0 envia mensagens exclusivas para cada processo
        for (int i = 1; i < size; i++) {
            // Cria a mensagem exclusiva para cada processo
            std::string mensagem = "Mensagem exclusiva para o processo " + std::to_string(i);
            MPI_Send(mensagem.c_str(), mensagem.size() + 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            std::cout << "Processo 0 enviou: \"" << mensagem << "\" para o processo " << i << std::endl;
        }
    } else {
        // Os outros processos recebem suas mensagens do processo 0
        const int tamanho_msg = 100;
        char mensagem[tamanho_msg];
        MPI_Recv(mensagem, tamanho_msg, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "Processo " << rank << " recebeu: \"" << mensagem << "\" do processo 0" << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
