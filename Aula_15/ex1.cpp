#include <mpi.h>
#include <iostream>

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    // Obtém o rank (ID) do processo
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        // Processo 0 envia "Ola" para o processo 1
        const char* mensagem_enviar = "Ola";
        MPI_Send(mensagem_enviar, 4, MPI_CHAR, 1, 0, MPI_COMM_WORLD);

        // Processo 0 recebe "Oi" do processo 1
        char mensagem_receber[3];
        MPI_Recv(mensagem_receber, 3, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Processo 0 recebeu: " << mensagem_receber << " do processo 1" << std::endl;
    } 
    else if (rank == 1) {
        // Processo 1 recebe "Ola" do processo 0
        char mensagem_receber[4];
        MPI_Recv(mensagem_receber, 4, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        std::cout << "Processo 1 recebeu: " << mensagem_receber << " do processo 0" << std::endl;

        // Processo 1 envia "Oi" de volta para o processo 0
        const char* mensagem_enviar = "Oi";
        MPI_Send(mensagem_enviar, 3, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
