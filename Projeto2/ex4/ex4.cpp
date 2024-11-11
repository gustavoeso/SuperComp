#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <filesystem>
#include <mpi.h>
#include <omp.h>

namespace fs = std::filesystem;

// Função para contar aminoácidos em uma sequência de RNA
void count_amino_acids(const std::string &sequence, std::unordered_map<std::string, int> &amino_acid_counts) {
    // Mapeamento de códons para aminoácidos
    std::unordered_map<std::string, std::string> codon_map = {
        {"CCA", "Prolina"}, {"CCG", "Prolina"}, {"CCU", "Prolina"}, {"CCC", "Prolina"},
        {"UCU", "Serina"}, {"UCA", "Serina"}, {"UCG", "Serina"}, {"UCC", "Serina"},
        {"CAG", "Glutamina"}, {"CAA", "Glutamina"},
        {"ACA", "Treonina"}, {"ACC", "Treonina"}, {"ACU", "Treonina"}, {"ACG", "Treonina"},
        {"AUG", "Metionina"},  // Códon de início
        {"UGA", "Stop"},       // Códon de parada
        {"UGC", "Cisteína"}, {"UGU", "Cisteína"},
        {"GUG", "Valina"}, {"GUA", "Valina"}, {"GUC", "Valina"}, {"GUU", "Valina"}
    };

    #pragma omp parallel for
    for (size_t i = 0; i < sequence.size(); i += 3) {
        if (i + 2 < sequence.size()) {
            std::string codon = sequence.substr(i, 3);
            if (codon_map.find(codon) != codon_map.end()) {
                #pragma omp critical
                amino_acid_counts[codon_map[codon]]++;
            }
        }
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::string input_dir = "../ex2/output";  // Diretório dos arquivos RNA
    std::vector<std::string> files;

    if (rank == 0) {
        for (const auto &entry : fs::directory_iterator(input_dir)) {
            if (entry.path().extension() == ".fa") {
                files.push_back(entry.path().string());
            }
        }
    }

    int num_files = files.size();
    MPI_Bcast(&num_files, 1, MPI_INT, 0, MPI_COMM_WORLD);

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

    std::unordered_map<std::string, int> local_amino_acid_counts;

    for (int i = rank; i < num_files; i += size) {
        std::ifstream file(files[i]);
        if (!file.is_open()) {
            std::cerr << "Erro ao abrir o arquivo: " << files[i] << std::endl;
            continue;
        }

        std::string line, sequence;
        while (std::getline(file, line)) {
            if (line[0] != '>') {
                sequence += line;
            }
        }
        file.close();

        std::unordered_map<std::string, int> amino_acid_counts;
        count_amino_acids(sequence, amino_acid_counts);

        for (const auto &pair : amino_acid_counts) {
            local_amino_acid_counts[pair.first] += pair.second;
        }

        std::cout << "Processo " << rank << " processou " << files[i] << " e encontrou:" << std::endl;
        for (const auto &pair : amino_acid_counts) {
            std::cout << "  " << pair.first << ": " << pair.second << std::endl;
        }
    }

    std::unordered_map<std::string, int> global_amino_acid_counts;

    for (const auto &amino_acid : {"Prolina", "Serina", "Glutamina", "Treonina", "Metionina", "Stop", "Cisteína", "Valina"}) {
        int local_count = local_amino_acid_counts[amino_acid];
        int global_count;
        MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            global_amino_acid_counts[amino_acid] = global_count;
        }
    }

    if (rank == 0) {
        std::cout << "\nTotal de aminoácidos encontrados em todos os arquivos:" << std::endl;
        for (const auto &pair : global_amino_acid_counts) {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
    }

    MPI_Finalize();
    return 0;
}
