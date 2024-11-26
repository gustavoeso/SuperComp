#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

// Função para calcular a média
double calculateMean(const std::vector<int>& data) {
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (size_t i = 0; i < data.size(); ++i) {
        sum += data[i];
    }

    return sum / data.size();
}

// Função para encontrar o maior valor
int calculateMax(const std::vector<int>& data) {
    int max_val = data[0];

    #pragma omp parallel for reduction(max:max_val)
    for (size_t i = 0; i < data.size(); ++i) {
        if (data[i] > max_val) {
            max_val = data[i];
        }
    }

    return max_val;
}

// Função para calcular o produto
long long calculateProduct(const std::vector<int>& data) {
    long long product = 1;

    #pragma omp parallel for reduction(*:product)
    for (size_t i = 0; i < data.size(); ++i) {
        product *= data[i];
    }

    return product;
}

int main() {
    const size_t VECTOR_SIZE = 100000;
    std::vector<int> data(VECTOR_SIZE);

    // Gerando valores aleatórios entre 1 e 100
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (size_t i = 0; i < VECTOR_SIZE; ++i) {
        data[i] = dis(gen);
    }

    double mean;
    int max_val;
    long long product;

    double start_time = omp_get_wtime(); // Início da medição de tempo

    // Paralelização das funções
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            mean = calculateMean(data);
        }
        #pragma omp section
        {
            max_val = calculateMax(data);
        }
        #pragma omp section
        {
            product = calculateProduct(data); // Pode estourar para vetores muito grandes (vamos ver se aguenta 1000000)
        }
    }

    double end_time = omp_get_wtime(); // Fim da medição de tempo

    std::cout << "Mean: " << mean << std::endl;
    std::cout << "Max: " << max_val << std::endl;
    std::cout << "Time taken: " << (end_time - start_time) << " seconds" << std::endl;

    return 0;
}
