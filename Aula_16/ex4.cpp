#include <mpi.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <numeric> // Para std::accumulate

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int tamanho_array = 16;  // Tamanho total do array
    int elementos_por_processo = tamanho_array / size;

    std::vector<float> array;
    std::vector<float> sub_array(elementos_por_processo);

    if (rank == 0) {
        // Processo raiz cria o array com valores aleatórios
        array.resize(tamanho_array);
        std::srand(std::time(nullptr));
        for (int i = 0; i < tamanho_array; ++i) {
            array[i] = static_cast<float>(std::rand() % 100); // Valores aleatórios entre 0 e 99
        }

        // Exibe o array inicial
        std::cout << "Array inicial no processo raiz: ";
        for (float val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Distribui partes do array para todos os processos
    MPI_Scatter(array.data(), elementos_por_processo, MPI_FLOAT,
                sub_array.data(), elementos_por_processo, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    // Cada processo calcula a média local
    float soma_local = std::accumulate(sub_array.begin(), sub_array.end(), 0.0f);
    float media_local = soma_local / elementos_por_processo;

    // Calcula a variância local
    float variancia_local = 0.0;
    for (float val : sub_array) {
        variancia_local += (val - media_local) * (val - media_local);
    }
    variancia_local /= elementos_por_processo;

    std::cout << "Processo " << rank << " calculou média local: " << media_local 
              << " e variância local: " << variancia_local << std::endl;

    // Recolhe médias e variâncias locais no processo raiz
    std::vector<float> medias_locais(size);
    std::vector<float> variancias_locais(size);

    MPI_Gather(&media_local, 1, MPI_FLOAT, medias_locais.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Gather(&variancia_local, 1, MPI_FLOAT, variancias_locais.data(), 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Processo raiz calcula a média global e o desvio padrão global
    if (rank == 0) {
        // Calcula a média global das médias locais
        float media_global = std::accumulate(medias_locais.begin(), medias_locais.end(), 0.0f) / size;

        // Calcula a variância global
        float variancia_global = std::accumulate(variancias_locais.begin(), variancias_locais.end(), 0.0f) / size;

        // Calcula o desvio padrão global
        float desvio_padrao_global = std::sqrt(variancia_global);

        std::cout << "Média global: " << media_global << std::endl;
        std::cout << "Desvio padrão global: " << desvio_padrao_global << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
