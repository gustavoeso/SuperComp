#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// Função para contar ocorrências do códon de início "AUG" em uma sequência de RNA
int countStartCodons(const std::string& sequence) {
    int count = 0;
    #pragma omp parallel for reduction(+:count)
    for (size_t i = 0; i <= sequence.size() - 3; i++) {
        if (sequence.substr(i, 3) == "AUG") {
            count++;
        }
    }
    return count;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Caminho dos arquivos de RNA gerados no exercício anterior
    std::string directory = "../ex2/output";
    std::vector<std::string> rnaFiles;

    // Apenas o processo principal (rank 0) lista os arquivos de RNA
    if (rank == 0) {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (entry.path().extension() == ".fa") {
                rnaFiles.push_back(entry.path().string());
            }
        }
    }

    // Broadcast do número de arquivos RNA para todos os processos
    int numFiles = rnaFiles.size();
    MPI_Bcast(&numFiles, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Distribuir a lista de arquivos RNA para todos os processos
    if (rank != 0) {
        rnaFiles.resize(numFiles);
    }
    for (int i = 0; i < numFiles; i++) {
        int length = (rank == 0) ? rnaFiles[i].size() : 0;
        MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank != 0) {
            rnaFiles[i].resize(length);
        }
        MPI_Bcast(&rnaFiles[i][0], length, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    double startTime = MPI_Wtime();

    // Contagem de códons de início local
    int localCount = 0;
    for (int i = rank; i < numFiles; i += size) {
        std::ifstream file(rnaFiles[i]);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo: " << rnaFiles[i] << std::endl;
            continue;
        }

        std::string line, sequence;
        while (std::getline(file, line)) {
            if (line[0] != '>') {
                sequence += line;  // Concatenar a sequência de RNA
            }
        }
        file.close();

        // Contar códons de início "AUG" na sequência
        localCount += countStartCodons(sequence);
        std::cout << "Processo " << rank << " encontrou " << localCount << " códons 'AUG' no arquivo: " << rnaFiles[i] << std::endl;
    }

    // Reduzir as contagens locais em uma soma global
    int globalCount = 0;
    MPI_Reduce(&localCount, &globalCount, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    double endTime = MPI_Wtime();
    double executionTime = endTime - startTime;

    if (rank == 0) {
        std::cout << "Total de proteínas inicializadas (códons 'AUG'): " << globalCount << std::endl;
        std::cout << "Tempo total de execução (Híbrido MPI + OpenMP): " << executionTime << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
