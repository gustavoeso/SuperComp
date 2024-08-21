#include <iostream>
#include <chrono>

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N

    // Medição de tempo para alocação dinâmica manual
    auto inicio = std::chrono::high_resolution_clock::now();

    // Alocação dinâmica de memória para uma matriz N x N
    int** matriz = new int*[N];
    for (int i = 0; i < N; ++i) {
        matriz[i] = new int[N];
    }

    // Inicialização e soma dos elementos da matriz
    long long soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;
            soma += matriz[i][j];
        }
    }

    // Liberação da memória alocada dinamicamente
    for (int i = 0; i < N; ++i) {
        delete[] matriz[i];
    }
    delete[] matriz;

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Soma dos elementos (alocação manual): " << soma << std::endl;
    std::cout << "Tempo de execução (alocação manual): " << duracao.count() << " segundos" << std::endl;

    return 0;
}
