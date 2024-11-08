#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

// Função para contar bases em uma sequência usando OpenMP
void count_bases_in_sequence(const std::string &sequence, int &countA, int &countT, int &countC, int &countG) {
    #pragma omp parallel for reduction(+:countA, countT, countC, countG)
    for (size_t i = 0; i < sequence.size(); ++i) {
        char base = std::toupper(sequence[i]);
        if (base == 'A') countA++;
        else if (base == 'T') countT++;
        else if (base == 'C') countC++;
        else if (base == 'G') countG++;
    }
}

// Função para contar bases em um arquivo
void count_bases_in_file(const std::string &filename, int &countA, int &countT, int &countC, int &countG) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string line, sequence;
    while (std::getline(file, line)) {
        if (line[0] != '>') {
            sequence += line;
        }
    }
    file.close();
    count_bases_in_sequence(sequence, countA, countT, countC, countG);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string directory = "../database";
    std::vector<std::string> files;

    // O processo 0 coleta a lista de arquivos e distribui entre os processos
    if (rank == 0) {
        for (const auto &entry : fs::directory_iterator(directory)) {
            if (entry.path().extension() == ".fa") {
                files.push_back(entry.path().string());
            }
        }
    }

    // Broadcast do número de arquivos para todos os processos
    int num_files = files.size();
    MPI_Bcast(&num_files, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast da lista de arquivos para todos os processos
    if (rank != 0) {
        files.resize(num_files);
    }
    for (int i = 0; i < num_files; i++) {
        int length = (rank == 0) ? files[i].size() : 0;
        MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank != 0) {
            files[i].resize(length);
        }
        MPI_Bcast(&files[i][0], length, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Contagem de bases locais para cada processo
    int local_countA = 0, local_countT = 0, local_countC = 0, local_countG = 0;

    // Processa os arquivos divididos entre os processos com OpenMP
    for (int i = rank; i < num_files; i += size) {
        count_bases_in_file(files[i], local_countA, local_countT, local_countC, local_countG);
    }

    // Reduz as contagens locais em uma soma global no processo 0
    int global_countA, global_countT, global_countC, global_countG;
    MPI_Reduce(&local_countA, &global_countA, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_countT, &global_countT, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_countC, &global_countC, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_countG, &global_countG, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // Apenas o processo 0 exibe o resultado final
    if (rank == 0) {
        std::cout << "Total de bases A: " << global_countA << std::endl;
        std::cout << "Total de bases T: " << global_countT << std::endl;
        std::cout << "Total de bases C: " << global_countC << std::endl;
        std::cout << "Total de bases G: " << global_countG << std::endl;
        std::cout << "Tempo total de execução: " << elapsed.count() << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
