#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

int main() {
    std::ifstream inputFile("input.txt");
    std::ofstream outputFile("output.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de entrada." << std::endl;
        return 1;
    }

    std::string line;
    int totalWords = 0;
    int totalLines = 0;
    std::unordered_map<std::string, int> wordCount;

    while (std::getline(inputFile, line)) {
        totalLines++;
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word) {
            totalWords++;
            wordCount[word]++;
        }
    }

    double averageWordsPerLine = static_cast<double>(totalWords) / totalLines;

    // Encontrar a palavra mais frequente
    std::string mostFrequentWord;
    int maxFrequency = 0;
    for (const auto& pair : wordCount) {
        if (pair.second > maxFrequency) {
            maxFrequency = pair.second;
            mostFrequentWord = pair.first;
        }
    }

    // Gravar as estatísticas no arquivo de saída
    outputFile << "Número total de palavras: " << totalWords << std::endl;
    outputFile << "Número total de linhas: " << totalLines << std::endl;
    outputFile << "Número médio de palavras por linha: " << averageWordsPerLine << std::endl;
    outputFile << "Palavra mais frequente: " << mostFrequentWord << " (ocorrências: " << maxFrequency << ")" << std::endl;

    inputFile.close();
    outputFile.close();

    std::cout << "As estatísticas foram gravadas no arquivo 'output.txt'." << std::endl;

    return 0;
}
