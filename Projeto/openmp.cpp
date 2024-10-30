#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

std::vector<std::vector<int>> carregarGrafo(const std::string &nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    std::vector<std::vector<int>> grafo;
    std::string linha;
    int linhaIndex = 0;

    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        int numero;
        std::vector<int> vizinhos;

        while (iss >> numero) {
            vizinhos.push_back(numero);
        }
        grafo.push_back(vizinhos);
        linhaIndex++;
    }

    return grafo;
}

std::vector<int> encontrarCliqueDoNo(int no, const std::vector<std::vector<int>> &grafo) {
    std::vector<int> clique = {no};

    for (int vizinho : grafo[no]) {
        bool completo = true;

        for (int membro : clique) {
            if (std::find(grafo[membro].begin(), grafo[membro].end(), vizinho) == grafo[membro].end()) {
                completo = false;
                break;
            }
        }

        if (completo) {
            clique.push_back(vizinho);
        }
    }

    return clique;
}

std::vector<int> encontrarCliqueMaxima(const std::vector<std::vector<int>> &grafo) {
    std::vector<int> cliqueMaxima;

    #pragma omp parallel
    {
        std::vector<int> cliqueLocal;

        #pragma omp for nowait
        for (int i = 0; i < grafo.size(); ++i) {
            std::vector<int> novaClique = encontrarCliqueDoNo(i, grafo);

            #pragma omp critical
            if (novaClique.size() > cliqueMaxima.size()) {
                cliqueMaxima = novaClique;
            }
        }
    }

    return cliqueMaxima;
}

int main() {
    std::string nomeArquivo = "grafos/grafo50.txt"; // Altere para o arquivo desejado
    auto grafo = carregarGrafo(nomeArquivo);

    auto inicio = std::chrono::high_resolution_clock::now();
    auto cliqueMaxima = encontrarCliqueMaxima(grafo);
    auto fim = std::chrono::high_resolution_clock::now();

    double tempoExecucao = std::chrono::duration<double>(fim - inicio).count();

    std::cout << "Clique máxima encontrada: ";
    for (int no : cliqueMaxima) std::cout << no << " ";
    std::cout << "\nTamanho da clique máxima: " << cliqueMaxima.size() << std::endl;
    std::cout << "Tempo total de execução: " << tempoExecucao << " segundos" << std::endl;

    return 0;
}
