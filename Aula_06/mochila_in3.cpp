#include <iostream>
#include <vector>
#include <fstream>

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
    ifstream arquivoEntrada("in3.txt"); // Substitua pelo nome de outro arquivo conforme necessário
    int numeroItens, capacidadeMochila;

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

    return 0;
}
