#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>
#include <string>
#include <filesystem>
#include <cctype>  // Para std::toupper

namespace fs = std::filesystem;

// Função para contar bases em uma sequência
void countBases(const std::string &sequence, int &countA, int &countT, int &countC, int &countG) {
    #pragma omp parallel for reduction(+:countA, countT, countC, countG)
    for (size_t i = 0; i < sequence.size(); ++i) {
        char base = std::toupper(sequence[i]);  // Converte para maiúsculas
        if (base == 'A') countA++;
        else if (base == 'T') countT++;
        else if (base == 'C') countC++;
        else if (base == 'G') countG++;
    }
}

// Função para processar um arquivo e contar bases
void processFile(const std::string &filename, int &countA, int &countT, int &countC, int &countG) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << "\n";
        return;
    }

    std::string line, sequence;
    while (std::getline(file, line)) {
        if (line[0] != '>') {
            sequence += line;  // Concatena todas as sequências do arquivo
        }
    }
    file.close();
    
    countBases(sequence, countA, countT, countC, countG);
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();  // Início da medição de tempo

    // Diretório contendo os arquivos de dados
    std::string directory = "../database";

    int totalA = 0, totalT = 0, totalC = 0, totalG = 0;

    // Itera sobre todos os arquivos na pasta `database` com extensão `.fa`
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".fa") {
            std::cout << "Processando arquivo: " << entry.path() << std::endl;
            processFile(entry.path().string(), totalA, totalT, totalC, totalG);
        }
    }

    // Exibe o resultado final
    std::cout << "Total de bases A: " << totalA << std::endl;
    std::cout << "Total de bases T: " << totalT << std::endl;
    std::cout << "Total de bases C: " << totalC << std::endl;
    std::cout << "Total de bases G: " << totalG << std::endl;

    auto end = std::chrono::high_resolution_clock::now();  // Fim da medição de tempo
    std::chrono::duration<double> duration = end - start;
    std::cout << "Tempo total de execução (OpenMP): " << duration.count() << " segundos" << std::endl;

    return 0;
}
