#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <string>
#include <fstream> // Inclui fstream para uso do ifstream

// Função para ler o grafo a partir do arquivo de entrada
std::vector<std::vector<int>> LerGrafo(const std::string& nomeArquivo, int& numVertices) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return {};
    }

    int numArestas;
    arquivo >> numVertices >> numArestas;

    // Inicializa a matriz de adjacência com 0s
    std::vector<std::vector<int>> grafo(numVertices, std::vector<int>(numVertices, 0));

    // Lê as arestas e preenche a matriz de adjacência
    for (int i = 0; i < numArestas; ++i) {
        int u, v;
        arquivo >> u >> v;
        grafo[u - 1][v - 1] = 1;
        grafo[v - 1][u - 1] = 1;  // Grafo não direcionado
    }

    arquivo.close();
    return grafo;
}

// Função recursiva para explorar todas as combinações e encontrar a clique máxima
void BuscaCliqueMaxima(const std::vector<std::vector<int>>& grafo, std::vector<int>& atualClique, 
                       std::vector<int>& cliqueMaxima, int numVertices, int vInicial) {
    // Se o clique atual é maior que a maior clique encontrada, atualizamos a clique máxima
    if (atualClique.size() > cliqueMaxima.size()) {
        cliqueMaxima = atualClique;
    }

    for (int v = vInicial; v < numVertices; ++v) {
        bool adjacenteATodos = true;

        // Verifica se o vértice v é adjacente a todos os vértices do clique atual
        for (int u : atualClique) {
            if (grafo[u][v] == 0) { // Se não for adjacente, quebra
                adjacenteATodos = false;
                break;
            }
        }

        if (adjacenteATodos) {
            atualClique.push_back(v); // Adiciona o vértice ao clique atual
            BuscaCliqueMaxima(grafo, atualClique, cliqueMaxima, numVertices, v + 1);
            atualClique.pop_back(); // Retira o vértice para explorar novas combinações
        }
    }
}

// Função principal para encontrar a clique máxima
std::vector<int> EncontrarCliqueMaxima(const std::vector<std::vector<int>>& grafo, int numVertices) {
    std::vector<int> cliqueMaxima;
    std::vector<int> atualClique;
    BuscaCliqueMaxima(grafo, atualClique, cliqueMaxima, numVertices, 0);
    return cliqueMaxima;
}

int main() {
    int numVertices;
    std::string nomeArquivo = "grafo.txt";
    
    // Lê o grafo do arquivo
    std::vector<std::vector<int>> grafo = LerGrafo(nomeArquivo, numVertices);

    auto inicioTotal = std::chrono::high_resolution_clock::now();
    
    // Encontra a clique máxima usando a abordagem exaustiva recursiva
    std::vector<int> cliqueMaxima = EncontrarCliqueMaxima(grafo, numVertices);
    
    auto duracaoTotal = std::chrono::duration_cast<std::chrono::seconds>(
                            std::chrono::high_resolution_clock::now() - inicioTotal).count();

    // Exibe o resultado no terminal
    std::cout << "Clique máxima encontrada: ";
    for (int v : cliqueMaxima) {
        std::cout << v + 1 << " "; // Exibe vértices da clique máxima
    }
    std::cout << std::endl;

    std::cout << "Tempo total de execução: " << duracaoTotal << " segundos" << std::endl;

    return 0;
}
