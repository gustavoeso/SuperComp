#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

struct Item {
    int peso;
    int valor;
};

void lerDados(const std::string& nomeArquivo, int& capacidade, std::vector<Item>& itens) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    int numeroItens;
    arquivo >> numeroItens >> capacidade;

    itens.resize(numeroItens);
    for (int i = 0; i < numeroItens; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
}

int heuristicaEmbaralharPreencher(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    std::random_shuffle(itens.begin(), itens.end());

    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        } else {
            break;
        }
    }
    return valorTotal;
}

int heuristicaSelecaoAleatoria(int capacidade, const std::vector<Item>& itens, double limiar, int& pesoTotal) {
    pesoTotal = 0;
    int valorTotal = 0;

    for (const auto& item : itens) {
        double probabilidade = static_cast<double>(rand()) / RAND_MAX;
        if (probabilidade > limiar && pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

int buscaExaustiva(int capacidade, const std::vector<Item>& itens, int n, int& pesoTotal) {
    if (n == 0 || capacidade == 0) {
        pesoTotal = 0;
        return 0;
    }

    int pesoSemItem, pesoComItem;

    if (itens[n-1].peso > capacidade) {
        return buscaExaustiva(capacidade, itens, n-1, pesoTotal);
    } else {
        int valorSemItem = buscaExaustiva(capacidade, itens, n-1, pesoSemItem);
        int valorComItem = itens[n-1].valor + buscaExaustiva(capacidade - itens[n-1].peso, itens, n-1, pesoComItem);

        if (valorComItem > valorSemItem) {
            pesoTotal = pesoComItem + itens[n-1].peso;
            return valorComItem;
        } else {
            pesoTotal = pesoSemItem;
            return valorSemItem;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./mochila <arquivo_de_entrada.txt>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];
    int capacidade;
    std::vector<Item> itens;

    lerDados(nomeArquivo, capacidade, itens);

    // Busca exaustiva
    int pesoTotalExaustivo = 0;
    clock_t inicio = clock();
    int valorExaustivo = buscaExaustiva(capacidade, itens, itens.size(), pesoTotalExaustivo);
    clock_t fim = clock();
    double tempoExaustivo = double(fim - inicio) / CLOCKS_PER_SEC;
    std::cout << "Busca Exaustiva: Valor = " << valorExaustivo << ", Peso Total = " << pesoTotalExaustivo 
              << ", Tempo = " << tempoExaustivo << "s" << std::endl;

    // Heurística 1: Embaralhar e Preencher
    for (int i = 0; i < 5; ++i) {
        int pesoTotalHeuristica1 = 0;
        inicio = clock();
        int valorHeuristica1 = heuristicaEmbaralharPreencher(capacidade, itens, pesoTotalHeuristica1);
        fim = clock();
        double tempoHeuristica1 = double(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "Heurística Aleatória (Embaralhar e Preencher), Execução " << i + 1 
                  << ": Valor = " << valorHeuristica1 << ", Peso Total = " << pesoTotalHeuristica1 
                  << ", Tempo = " << tempoHeuristica1 << "s" << std::endl;
    }

    // Heurística 2: Seleção Aleatória Baseada em Probabilidade
    for (int i = 0; i < 5; ++i) {
        int pesoTotalHeuristica2 = 0;
        inicio = clock();
        int valorHeuristica2 = heuristicaSelecaoAleatoria(capacidade, itens, 0.5, pesoTotalHeuristica2);
        fim = clock();
        double tempoHeuristica2 = double(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "Heurística Aleatória (Seleção Baseada em Probabilidade), Execução " << i + 1 
                  << ": Valor = " << valorHeuristica2 << ", Peso Total = " << pesoTotalHeuristica2 
                  << ", Tempo = " << tempoHeuristica2 << "s" << std::endl;
    }

    return 0;
}
