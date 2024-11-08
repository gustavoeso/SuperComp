#include <iostream>
#include <fstream>
#include <mpi.h>
#include <omp.h>
#include <string>
#include <vector>
#include <filesystem>

// Função para processar um arquivo e converter DNA para RNA
std::string processarArquivo(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir o arquivo: " << filename << "\n";
        return "";
    }

    std::string line, sequenciaRNA;
    while (std::getline(file, line)) {
        if (line[0] != '>') {  // Ignora cabeçalhos
            for (char base : line) {
                sequenciaRNA += (base == 'T') ? 'U' : toupper(base);
            }
        }
    }
    file.close();
    return sequenciaRNA;
}

// Função para salvar cada RNA gerado em um arquivo individual na pasta "output"
void salvarRNA(const std::string &rnaSequencia, const std::string &filename) {
    std::filesystem::create_directory("output");  // Cria a pasta 'output' se não existir
    std::string outputFilename = "output/" + filename + ".rna.fa";

    std::ofstream arquivoSaida(outputFilename);
    if (arquivoSaida.is_open()) {
        arquivoSaida << rnaSequencia << "\n";
        arquivoSaida.close();
        std::cout << "Arquivo RNA salvo em: " << outputFilename << std::endl;
    } else {
        std::cerr << "Erro ao abrir o arquivo para salvar o RNA: " << outputFilename << "\n";
    }
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const std::string directory = "../database";
    std::vector<std::string> files;

    if (rank == 0) {
        for (const auto &entry : std::filesystem::directory_iterator(directory)) {
            if (entry.path().extension() == ".fa") {
                files.push_back(entry.path().string());
            }
        }
    }

    int num_files = files.size();
    MPI_Bcast(&num_files, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank != 0) files.resize(num_files);

    for (int i = 0; i < num_files; i++) {
        int length = (rank == 0) ? files[i].size() : 0;
        MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank != 0) files[i].resize(length);
        MPI_Bcast(&files[i][0], length, MPI_CHAR, 0, MPI_COMM_WORLD);
    }

    double start_time = MPI_Wtime();

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = rank; i < num_files; i += size) {
            std::string rna = processarArquivo(files[i]);
            std::string filename = std::filesystem::path(files[i]).stem();
            salvarRNA(rna, filename);
        }
    }

    if (rank == 0) {
        double end_time = MPI_Wtime();
        std::cout << "Tempo total de execução (Híbrido MPI + OpenMP): " << (end_time - start_time) << " segundos\n";
    }

    MPI_Finalize();
    return 0;
}
