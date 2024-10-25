#include <mpi.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric> // Para std::accumulate

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int tamanho_array = 16;  // Tamanho total do array (exemplo)
    int elementos_por_processo = tamanho_array / size;

    std::vector<int> array;
    std::vector<int> sub_array(elementos_por_processo);

    if (rank == 0) {
        // Processo raiz inicializa o array com valores aleatórios
        array.resize(tamanho_array);
        std::srand(std::time(nullptr));
        for (int i = 0; i < tamanho_array; ++i) {
            array[i] = std::rand() % 100; // Valores aleatórios entre 0 e 99
        }

        // Exibe o array inicial
        std::cout << "Array inicial no processo raiz: ";
        for (int val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Distribui partes do array para todos os processos
    MPI_Scatter(array.data(), elementos_por_processo, MPI_INT,
                sub_array.data(), elementos_por_processo, MPI_INT,
                0, MPI_COMM_WORLD);

    // Cada processo calcula a média de sua parte do array
    double media_local = std::accumulate(sub_array.begin(), sub_array.end(), 0.0) / elementos_por_processo;
    std::cout << "Processo " << rank << " calculou média local: " << media_local << std::endl;

    // Coleta as médias locais no processo raiz
    std::vector<double> medias_locais;
    if (rank == 0) {
        medias_locais.resize(size);
    }
    MPI_Gather(&media_local, 1, MPI_DOUBLE, medias_locais.data(), 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Processo raiz calcula a média global
    if (rank == 0) {
        double media_global = std::accumulate(medias_locais.begin(), medias_locais.end(), 0.0) / size;
        std::cout << "Média global calculada no processo raiz: " << media_global << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
