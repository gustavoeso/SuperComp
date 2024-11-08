#include <iostream>
#include <fstream>
#include <mpi.h>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>  // Para std::toupper
#include <algorithm>  // Para std::sort

namespace fs = std::filesystem;

// Função para contar bases em um arquivo com conversão para maiúsculas
void count_bases_in_file(const std::string &filename, int &countA, int &countT, int &countC, int &countG) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '>') continue;  // Ignora linhas de cabeçalho ou vazias
        for (char base : line) {
            base = std::toupper(base);  // Converte para maiúsculas
            if (base == 'A') countA++;
            else if (base == 'T') countT++;
            else if (base == 'C') countC++;
            else if (base == 'G') countG++;
        }
    }
    file.close();
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    double start = MPI_Wtime();  // Início da medição de tempo

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Diretório contendo os arquivos de dados
    std::string directory = "../database";
    std::vector<std::string> files;

    // Todos os processos obtêm a lista de arquivos e a ordenam
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".fa") {
            files.push_back(entry.path().string());
        }
    }

    // Ordena a lista de arquivos para garantir a mesma ordem em todos os processos
    std::sort(files.begin(), files.end());

    int num_files = files.size();

    // Calcula os índices de início e fim para cada processo
    int files_per_process = num_files / size;
    int remainder = num_files % size;
    int start_idx = rank * files_per_process + std::min(rank, remainder);
    int end_idx = start_idx + files_per_process + (rank < remainder ? 1 : 0);

    // Para depuração: exibe quais arquivos cada processo está processando
    std::cout << "Processo " << rank << " processando arquivos de índice " << start_idx << " a " << end_idx - 1 << std::endl;

    // Contagem de bases locais para cada processo
    int local_countA = 0, local_countT = 0, local_countC = 0, local_countG = 0;

    for (int i = start_idx; i < end_idx; ++i) {
        std::cout << "Processo " << rank << " processando arquivo: " << files[i] << std::endl;
        count_bases_in_file(files[i], local_countA, local_countT, local_countC, local_countG);
    }

    // Reduz as contagens locais em uma soma global no processo 0
    int global_countA = 0, global_countT = 0, global_countC = 0, global_countG = 0;
    MPI_Reduce(&local_countA, &global_countA, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_countT, &global_countT, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_countC, &global_countC, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_countG, &global_countG, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Apenas o processo 0 exibe o resultado final
    if (rank == 0) {
        std::cout << "Total de bases A: " << global_countA << std::endl;
        std::cout << "Total de bases T: " << global_countT << std::endl;
        std::cout << "Total de bases C: " << global_countC << std::endl;
        std::cout << "Total de bases G: " << global_countG << std::endl;
    }

    double end = MPI_Wtime();  // Fim da medição de tempo
    if (rank == 0) {  // Apenas o processo 0 exibe o tempo total
        std::cout << "Tempo total de execução (MPI): " << end - start << " segundos" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
