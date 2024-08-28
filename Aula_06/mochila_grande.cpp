#include <iostream>
#include <vector>
#include <fstream>
#include <chrono>     // Inclui a biblioteca de medição de tempo do C++ (necessária para medir o tempo de execução).

using namespace std;

// Função recursiva para resolver o problema da mochila
int knapsack(int capacidadeMochila, vector<int> &pesosItens, vector<int> &valoresItens, int numeroItens) {
    // Caso base: sem itens ou capacidade da mochila é 0
    if (numeroItens == 0 || capacidadeMochila == 0)
        return 0;

    // Se o peso do último item é maior que a capacidade da mochila, ele não pode ser incluído
    if (pesosItens[numeroItens - 1] > capacidadeMochila)
        return knapsack(capacidadeMochila, pesosItens, valoresItens, numeroItens - 1);

    // Retorna o máximo valor obtido ao incluir ou não incluir o item atual
    else
        return max(valoresItens[numeroItens - 1] + knapsack(capacidadeMochila - pesosItens[numeroItens - 1], pesosItens, valoresItens, numeroItens - 1),
                   knapsack(capacidadeMochila, pesosItens, valoresItens, numeroItens - 1));
}

int main() {

    ifstream arquivoEntrada("pequeno.txt"); // Substitua pelo nome de outro arquivo conforme necessário
    int numeroItens, capacidadeMochila;

    // Marca o tempo de início da multiplicação das matrizes usando o relógio de alta resolução.
    auto start = std::chrono::high_resolution_clock::now();

    // Lendo o número de itens e a capacidade da mochila
    arquivoEntrada >> numeroItens >> capacidadeMochila;

    vector<int> pesosItens(numeroItens);
    vector<int> valoresItens(numeroItens);

    // Lendo pesos e valores dos itens
    for (int i = 0; i < numeroItens; i++) {
        arquivoEntrada >> pesosItens[i] >> valoresItens[i];
    }

    arquivoEntrada.close();

    cout << "Valor máximo possível: " << knapsack(capacidadeMochila, pesosItens, valoresItens, numeroItens) << endl;


    // Marca o tempo de término da multiplicação.
    auto end = std::chrono::high_resolution_clock::now();

    // Calcula a duração da multiplicação subtraindo o tempo de início do tempo de término, armazenando o resultado em 'duration'.
   std::chrono::duration<double> duration = end - start;

   // Exibe o tempo de execução da multiplicação de matrizes no console.
   std::cout << "Tempo de execução para a multiplicação de matrizes em C++: " << duration.count() << " segundos" << std::endl;

    return 0;
}
