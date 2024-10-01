#include <iostream>
#include <vector>
#include <algorithm>  // Para sort
#include <fstream>    // Para ler arquivos
#include <chrono>     // Para medir o tempo de execução
using namespace std;

// Função para resolver o problema do subconjunto de soma usando heurística
bool subsetSum(const vector<int>& set, int target) {
    // Criar uma cópia do conjunto para não modificar o original
    vector<int> sortedSet = set;
    
    // Passo 1: Ordenar os números em ordem decrescente
    sort(sortedSet.begin(), sortedSet.end(), greater<int>());

    // Variável para armazenar a soma do subconjunto candidato
    int currentSum = 0;

    // Passo 2: Heurística de aproximação
    for (int num : sortedSet) {
        if (currentSum + num <= target) {
            currentSum += num;  // Incluir o número no subconjunto candidato
        }

        // Se atingimos o valor alvo, retornar true
        if (currentSum == target) {
            return true;
        }
    }

    // Se não conseguimos atingir a soma exata, retornar false
    return false;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <arquivo de entrada>" << endl;
        return 1;
    }

    // Abrir o arquivo de entrada
    ifstream inputFile(argv[1]);
    if (!inputFile) {
        cerr << "Erro ao abrir o arquivo de entrada!" << endl;
        return 1;
    }

    // Ler os números do arquivo
    vector<int> set;
    int num;
    while (inputFile >> num) {
        set.push_back(num);
    }

    // Fechar o arquivo após a leitura
    inputFile.close();

    // O último número do arquivo será o valor alvo
    int target = set.back();
    set.pop_back();  // Remover o valor alvo do conjunto

    // Medir o tempo de execução
    auto start = chrono::steady_clock::now();

    // Executar o algoritmo de subconjunto de soma
    bool result = subsetSum(set, target);

    auto end = chrono::steady_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;

    // Exibir o resultado
    if (result) {
        cout << "Há um subconjunto cuja soma é igual a " << target << "." << endl;
    } else {
        cout << "Não há subconjunto cuja soma seja igual a " << target << "." << endl;
    }

    // Exibir o tempo de execução
    cout << "Tempo de execução: " << elapsed_seconds.count() << " segundos." << endl;

    return 0;
}
