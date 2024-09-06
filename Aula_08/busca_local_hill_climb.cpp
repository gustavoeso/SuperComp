#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>

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

// Função que gera uma solução aleatória embaralhada (pode ser usada em várias abordagens)
int heuristicaEmbaralharPreencher(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    std::random_shuffle(itens.begin(), itens.end());

    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        } else {
            break;
        }
    }
    return valorTotal;
}

// Função para a Mochila Cheia (Busca Local simples)
int mochilaCheia(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Gera uma solução aleatória e tenta preencher a mochila
    std::random_shuffle(itens.begin(), itens.end());

    pesoTotal = 0;
    int valorTotal = 0;
    for (const auto& item : itens) {
        if (pesoTotal + item.peso <= capacidade) {
            pesoTotal += item.peso;
            valorTotal += item.valor;
        }
    }
    return valorTotal;
}

// Função para Substituição de Objeto corrigida
int substituicaoDeObjeto(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Inicializa a solução aleatória e ajusta com a função mochilaCheia
    int valorTotal = mochilaCheia(capacidade, itens, pesoTotal);

    bool houveTroca;
    int tentativas = 0;

    // Vetor para armazenar quais itens estão na mochila (1 = presente, 0 = ausente)
    std::vector<int> estaNaMochila(itens.size(), 0);

    // Preencher o vetor com base na solução inicial
    for (size_t i = 0; i < itens.size(); ++i) {
        if (pesoTotal - itens[i].peso >= 0) {
            estaNaMochila[i] = 1;
        }
    }

    do {
        houveTroca = false;

        // Itera sobre os itens presentes na mochila
        for (size_t i = 0; i < itens.size(); ++i) {
            if (estaNaMochila[i] == 0) continue; // Item não está na mochila

            // Tenta substituir com outros itens fora da mochila
            for (size_t j = 0; j < itens.size(); ++j) {
                if (estaNaMochila[j] == 1) continue; // O item já está na mochila, não pode substituir

                // Verifica se a troca manterá o peso dentro da capacidade
                if (pesoTotal - itens[i].peso + itens[j].peso <= capacidade && itens[j].valor > itens[i].valor) {
                    // Calcula o novo peso e valor após a troca
                    int novoPeso = pesoTotal - itens[i].peso + itens[j].peso;
                    int novoValor = valorTotal - itens[i].valor + itens[j].valor;

                    // Atualiza se houver melhoria
                    if (novoValor > valorTotal) {
                        pesoTotal = novoPeso;
                        valorTotal = novoValor;

                        // Realiza a troca
                        estaNaMochila[i] = 0; // Remove o item i
                        estaNaMochila[j] = 1; // Adiciona o item j

                        houveTroca = true;
                        break; // Sai para realizar uma troca por vez
                    }
                }
            }

            if (houveTroca) break; // Sai do laço externo se houve troca
        }

        tentativas++;
        if (tentativas > 10000) {
            std::cerr << "Número máximo de iterações atingido. Saindo para evitar loops infinitos." << std::endl;
            break;
        }

    } while (houveTroca);

    return valorTotal;
}

// Função para Hill Climbing
int hillClimbing(int capacidade, std::vector<Item>& itens, int& pesoTotal) {
    // Gera solução inicial (aleatória) como string binária
    std::vector<int> solucao(itens.size(), 0); // 0 = não incluso, 1 = incluso
    int valorAtual = heuristicaEmbaralharPreencher(capacidade, itens, pesoTotal);
    int melhorValor = valorAtual;

    bool encontrouMelhor = true;
    while (encontrouMelhor) {
        encontrouMelhor = false;

        // Gera vizinhos mudando um bit da solução (0 vira 1 ou 1 vira 0)
        for (size_t i = 0; i < solucao.size(); ++i) {
            int novoPeso = pesoTotal;
            int novoValor = valorAtual;

            if (solucao[i] == 0 && (novoPeso + itens[i].peso) <= capacidade) {
                novoPeso += itens[i].peso;
                novoValor += itens[i].valor;
            } else if (solucao[i] == 1) {
                novoPeso -= itens[i].peso;
                novoValor -= itens[i].valor;
            }

            // Se o novo vizinho for melhor, atualiza a solução
            if (novoValor > melhorValor) {
                solucao[i] = 1 - solucao[i]; // Alterna o bit
                melhorValor = novoValor;
                pesoTotal = novoPeso;
                encontrouMelhor = true;
            }
        }
    }
    return melhorValor;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: ./busca_local_hill_climb <arquivo_de_entrada.txt>" << std::endl;
        return 1;
    }

    std::string nomeArquivo = argv[1];
    int capacidade;
    std::vector<Item> itens;

    lerDados(nomeArquivo, capacidade, itens);

    // Heurística Aleatória (Embaralhar e Preencher)
    for (int i = 0; i < 5; ++i) {
        int pesoTotalHeuristica1 = 0;
        clock_t inicio = clock();
        int valorHeuristica1 = heuristicaEmbaralharPreencher(capacidade, itens, pesoTotalHeuristica1);
        clock_t fim = clock();
        double tempoHeuristica1 = double(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "Heurística Aleatória (Embaralhar e Preencher), Execução " << i + 1 
                  << ": Valor = " << valorHeuristica1 << ", Peso Total = " << pesoTotalHeuristica1 
                  << ", Tempo = " << tempoHeuristica1 << "s" << std::endl;
    }

    // Mochila Cheia
    for (int i = 0; i < 10; ++i) {
        int pesoTotalMochilaCheia = 0;
        clock_t inicio = clock();
        int valorMochilaCheia = mochilaCheia(capacidade, itens, pesoTotalMochilaCheia);
        clock_t fim = clock();
        double tempoMochilaCheia = double(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "Busca Local (Mochila Cheia), Execução " << i + 1 
                  << ": Valor = " << valorMochilaCheia << ", Peso Total = " << pesoTotalMochilaCheia 
                  << ", Tempo = " << tempoMochilaCheia << "s" << std::endl;
    }

    // Substituição de Objeto
    for (int i = 0; i < 10; ++i) {
        int pesoTotalSubstituicao = 0;
        clock_t inicio = clock();
        int valorSubstituicao = substituicaoDeObjeto(capacidade, itens, pesoTotalSubstituicao);
        clock_t fim = clock();
        double tempoSubstituicao = double(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "Busca Local (Substituição de Objeto), Execução " << i + 1 
                  << ": Valor = " << valorSubstituicao << ", Peso Total = " << pesoTotalSubstituicao 
                  << ", Tempo = " << tempoSubstituicao << "s" << std::endl;
    }

    // Hill Climbing
    for (int i = 0; i < 10; ++i) {
        int pesoTotalHillClimbing = 0;
        clock_t inicio = clock();
        int valorHillClimbing = hillClimbing(capacidade, itens, pesoTotalHillClimbing);
        clock_t fim = clock();
        double tempoHillClimbing = double(fim - inicio) / CLOCKS_PER_SEC;
        std::cout << "Hill Climbing, Execução " << i + 1 
                  << ": Valor = " << valorHillClimbing << ", Peso Total = " << pesoTotalHillClimbing 
                  << ", Tempo = " << tempoHillClimbing << "s" << std::endl;
    }

    return 0;
}
