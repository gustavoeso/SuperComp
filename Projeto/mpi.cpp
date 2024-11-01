#include <mpi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
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

std::vector<int> encontrarCliqueDoNo(int no, const std::vector<std::vector<int>> &grafo) {
    std::vector<int> clique = {no};
    for (int vizinho : grafo[no]) {
        bool parteDaClique = true;
        for (int membro : clique) {
            if (std::find(grafo[membro].begin(), grafo[membro].end(), vizinho) == grafo[membro].end()) {
                parteDaClique = false;
                break;
            }
        }
        if (parteDaClique) {
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

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string arquivo = "grafos/grafo150.txt";
    std::vector<std::vector<int>> grafo = carregarGrafo(arquivo);

    std::vector<int> cliqueMaximaLocal;
    int maxCliqueSizeLocal = 0;
    auto start = std::chrono::high_resolution_clock::now();

    int chunkSize = grafo.size() / size;
    int inicio = rank * chunkSize;
    int fim = (rank == size - 1) ? grafo.size() : inicio + chunkSize;

    for (int i = inicio; i < fim; ++i) {
        auto clique = encontrarCliqueDoNo(i, grafo);
        if (clique.size() > cliqueMaximaLocal.size()) {
            cliqueMaximaLocal = clique;
            maxCliqueSizeLocal = clique.size();
        }
    }

    int maxCliqueSizeGlobal;
    MPI_Allreduce(&maxCliqueSizeLocal, &maxCliqueSizeGlobal, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

    std::vector<int> cliqueMaximaGlobal(maxCliqueSizeGlobal, -1);
    if (maxCliqueSizeLocal == maxCliqueSizeGlobal) {
        cliqueMaximaGlobal = cliqueMaximaLocal;
    }

    MPI_Bcast(cliqueMaximaGlobal.data(), maxCliqueSizeGlobal, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duracao = end - start;
        std::cout << "Clique máxima encontrada: ";
        for (int no : cliqueMaximaGlobal) std::cout << no << " ";
        std::cout << "\nTamanho da clique máxima: " << cliqueMaximaGlobal.size() << "\n";
        std::cout << "Tempo total de execução: " << duracao.count() << " segundos\n";
    }

    MPI_Finalize();
    return 0;
}