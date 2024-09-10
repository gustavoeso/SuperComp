#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

// Estrutura que representa os itens
struct Item {
    int peso;
    int valor;
};

// Função para ler os dados da mochila de um arquivo
void lerDados(const std::string& nomeArquivo, int& capacidade, std::vector<Item>& itens) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo) {
        std::cerr << "Erro ao abrir o arquivo " << nomeArquivo << std::endl;
        return;
    }

    int numeroItens;
    arquivo >> numeroItens >> capacidade;

    itens.resize(numeroItens);
    for (int i = 0; i < numeroItens; ++i) {
        arquivo >> itens[i].peso >> itens[i].valor;
    }
}

// Função para comparar itens pelo valor (mais caro primeiro)
bool compararValor(const Item& a, const Item& b) {
    return a.valor > b.valor;
}

// Função para comparar itens pelo peso (mais leve primeiro)
bool compararPeso(const Item& a, const Item& b) {
    return a.peso < b.peso;
}

// Função da heurística "Mais caro primeiro"
int maisCaroPrimeiro(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Ordena os itens por valor (do mais caro para o mais barato)
    std::sort(itens.begin(), itens.end(), compararValor);

    pesoTotal = 0;
    int valorTotal = 0;

    // Tenta incluir os itens na mochila, começando pelos mais caros
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        }
    }

    return valorTotal;
}

// Função da heurística "Mais leve primeiro"
int maisLevePrimeiro(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Ordena os itens por peso (do mais leve para o mais pesado)
    std::sort(itens.begin(), itens.end(), compararPeso);

    pesoTotal = 0;
    int valorTotal = 0;

    // Tenta incluir os itens na mochila, começando pelos mais leves
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        }
    }

    return valorTotal;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./comparar_heuristicas <arquivo_de_entrada.txt>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];
    int capacidade;
    std::vector<Item> itens;

    // Lê os dados do arquivo
    lerDados(nomeArquivo, capacidade, itens);

    // Execução da heurística "Mais caro primeiro"
    int pesoTotalCaro = 0;
    clock_t inicioCaro = clock();
    int valorTotalCaro = maisCaroPrimeiro(capacidade, itens, pesoTotalCaro);
    clock_t fimCaro = clock();
    double tempoExecucaoCaro = double(fimCaro - inicioCaro) / CLOCKS_PER_SEC;

    // Execução da heurística "Mais leve primeiro"
    int pesoTotalLeve = 0;
    clock_t inicioLeve = clock();
    int valorTotalLeve = maisLevePrimeiro(capacidade, itens, pesoTotalLeve);
    clock_t fimLeve = clock();
    double tempoExecucaoLeve = double(fimLeve - inicioLeve) / CLOCKS_PER_SEC;

    // Exibe os resultados para a heurística "Mais caro primeiro"
    std::cout << "Heurística Mais Caro Primeiro: \n";
    std::cout << "Valor Total = " << valorTotalCaro << ", Peso Total = " << pesoTotalCaro
              << ", Tempo = " << tempoExecucaoCaro << "s\n";

    // Exibe os resultados para a heurística "Mais leve primeiro"
    std::cout << "Heurística Mais Leve Primeiro: \n";
    std::cout << "Valor Total = " << valorTotalLeve << ", Peso Total = " << pesoTotalLeve
              << ", Tempo = " << tempoExecucaoLeve << "s\n";

    return 0;
}
