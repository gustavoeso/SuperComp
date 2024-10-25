#include <mpi.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char** argv) {
    // Inicializa o ambiente MPI
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int tamanho_array = 16;  // Tamanho total do array (exemplo)
    int elementos_por_processo = tamanho_array / size;

    std::vector<float> array;
    float max_valor = 0.0;

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

        // Calcula o valor máximo com opção de usar OpenMP
        #ifdef _OPENMP
        #pragma omp parallel for reduction(max:max_valor)
        #endif
        for (int i = 0; i < tamanho_array; ++i) {
            if (array[i] > max_valor) {
                max_valor = array[i];
            }
        }

        std::cout << "Valor máximo calculado no processo raiz: " << max_valor << std::endl;
    }

    // Transmite o valor máximo para todos os processos
    MPI_Bcast(&max_valor, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);

    // Cria um sub-array para cada processo
    std::vector<float> sub_array(elementos_por_processo);

    // Distribui partes do array para todos os processos
    MPI_Scatter(array.data(), elementos_por_processo, MPI_FLOAT,
                sub_array.data(), elementos_por_processo, MPI_FLOAT,
                0, MPI_COMM_WORLD);

    // Cada processo normaliza seu segmento do array
    for (int i = 0; i < elementos_por_processo; ++i) {
        sub_array[i] /= max_valor;
    }
    std::cout << "Processo " << rank << " normalizou seu segmento do array." << std::endl;

    // Recolhe os segmentos normalizados no processo raiz
    MPI_Gather(sub_array.data(), elementos_por_processo, MPI_FLOAT,
               array.data(), elementos_por_processo, MPI_FLOAT,
               0, MPI_COMM_WORLD);

    // Processo raiz exibe o array normalizado
    if (rank == 0) {
        std::cout << "Array normalizado no processo raiz: ";
        for (float val : array) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }

    // Finaliza o ambiente MPI
    MPI_Finalize();

    return 0;
}
