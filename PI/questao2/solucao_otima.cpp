#include <iostream>
#include <vector>
#include <fstream>
#include <chrono> // Para medir o tempo de execução

using namespace std;

// Função recursiva para verificar todos os subconjuntos possíveis
bool subsetSum(const vector<int>& set, int n, int sum) {
    // Caso base: Se a soma for 0, encontramos um subconjunto
    if (sum == 0) {
        return true;
    }

    // Se não houver mais elementos e a soma não for 0, retorne false
    if (n == 0) {
        return false;
    }

    // Se o último elemento é maior que a soma, ignorar este elemento
    if (set[n - 1] > sum) {
        return subsetSum(set, n - 1, sum);
    }

    // Verificar duas opções: incluir o último elemento ou não
    return subsetSum(set, n - 1, sum) || subsetSum(set, n - 1, sum - set[n - 1]);
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
    set.pop_back(); // Remover o valor alvo do conjunto

    // Medir o tempo de execução
    auto start = chrono::steady_clock::now();

    // Executar o algoritmo de subconjunto de soma
    bool result = subsetSum(set, set.size(), target);

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
