#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <chrono>

std::vector<std::vector<int>> carregarGrafo(const std::string &arquivo) {
    std::ifstream inputFile(arquivo);
    std::vector<std::vector<int>> grafo;
    std::string linha;
    while (getline(inputFile, linha)) {
        std::istringstream iss(linha);
        int v1, v2;
        iss >> v1 >> v2;
        while (grafo.size() <= std::max(v1, v2)) {
            grafo.emplace_back();
        }
        grafo[v1].push_back(v2);
        grafo[v2].push_back(v1);
    }
    return grafo;
}

std::vector<int> encontrarCliqueDoNo(int no, const std::vector<std::vector<int>>& grafo) {
    std::vector<int> clique = {no};
    for (int vizinho : grafo[no]) {
        bool fazParteClique = true;
        for (int membro : clique) {
            if (std::find(grafo[membro].begin(), grafo[membro].end(), vizinho) == grafo[membro].end()) {
                fazParteClique = false;
                break;
            }
        }
        if (fazParteClique) {
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
    std::string arquivo = "grafos/grafo150.txt";
    auto grafo = carregarGrafo(arquivo);
    std::vector<int> cliqueMaxima;
    auto start = std::chrono::high_resolution_clock::now();

    #pragma omp parallel
    {
        std::vector<int> cliqueLocal;
        #pragma omp for
        for (int i = 0; i < grafo.size(); ++i) {
            auto clique = encontrarCliqueDoNo(i, grafo);
            #pragma omp critical
            {
                if (clique.size() > cliqueMaxima.size()) {
                    cliqueMaxima = clique;
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = end - start;

    std::cout << "Clique máxima encontrada: ";
    for (int no : cliqueMaxima) std::cout << no << " ";
    std::cout << "\nTamanho da clique máxima: " << cliqueMaxima.size() << "\n";
    std::cout << "Tempo total de execução: " << duracao.count() << " segundos\n";

    return 0;
}