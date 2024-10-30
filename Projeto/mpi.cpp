#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::vector<std::vector<int>> carregarGrafo(const std::string &nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    std::vector<std::vector<int>> grafo;
    std::string linha;

    while (std::getline(arquivo, linha)) {
        std::istringstream iss(linha);
        int numero;
        std::vector<int> vizinhos;

        while (iss >> numero) {
            vizinhos.push_back(numero);
        }
        grafo.push_back(vizinhos);
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

std::vector<int> encontrarCliqueMaxima(const std::vector<std::vector<int>> &grafo, int rank, int size) {
    std::vector<int> cliqueMaxima;

    for (int i = rank; i < grafo.size(); i += size) {
        std::vector<int> novaClique = encontrarCliqueDoNo(i, grafo);

        if (novaClique.size() > cliqueMaxima.size()) {
            cliqueMaxima = novaClique;
        }
    }

    return cliqueMaxima;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string nomeArquivo = "grafos/grafo50.txt"; // Altere para o arquivo desejado
    auto grafo = carregarGrafo(nomeArquivo);

    auto inicio = MPI_Wtime();
    auto cliqueLocal = encontrarCliqueMaxima(grafo, rank, size);

    int cliqueMaximaSize = cliqueLocal.size();
    std::vector<int> cliqueMaxima;

    for (int i = 0; i < cliqueMaximaSize; ++i) {
        cliqueMaxima.push_back(cliqueLocal[i]);
    }

    auto fim = MPI_Wtime();
    double tempoExecucao = fim - inicio;

    if (rank == 0) {
        std::cout << "Clique máxima encontrada: ";
        for (int no : cliqueMaxima) std::cout << no << " ";
        std::cout << "\nTamanho da clique máxima: " << cliqueMaxima.size() << std::endl;
        std::cout << "Tempo total de execução: " << tempoExecucao << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
