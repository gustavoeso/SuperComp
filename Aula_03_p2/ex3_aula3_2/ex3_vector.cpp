#include <iostream>
#include <vector>
#include <chrono>

int main() {
    int N = 1000;  // Define o tamanho da matriz como N x N

    // Medição de tempo para alocação usando std::vector
    auto inicio = std::chrono::high_resolution_clock::now();

    // Alocação de memória para uma matriz N x N usando std::vector
    std::vector<std::vector<int>> matriz(N, std::vector<int>(N));

    // Inicialização e soma dos elementos da matriz
    long long soma = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matriz[i][j] = i + j;
            soma += matriz[i][j];
        }
    }

    auto fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = fim - inicio;

    std::cout << "Soma dos elementos (std::vector): " << soma << std::endl;
    std::cout << "Tempo de execução (std::vector): " << duracao.count() << " segundos" << std::endl;

    return 0;
}
