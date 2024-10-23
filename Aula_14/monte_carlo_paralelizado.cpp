#include <iostream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>

// Função para gerar um número aleatório entre 0 e 1
double random_double() {
    return static_cast<double>(rand()) / RAND_MAX;
}

int main() {
    int N = 100000;  // Número de pontos
    int pontos_no_circulo = 0;
    
    // Semente para o gerador de números aleatórios
    std::srand(std::time(nullptr));

    // Início da medição do tempo
    double start_time = omp_get_wtime();

    #pragma omp parallel for reduction(+:pontos_no_circulo)
    for (int i = 0; i < N; i++) {
        // Gerar pontos (x, y) aleatórios no quadrado [-1, 1] x [-1, 1]
        double x = random_double() * 2.0 - 1.0;  // Valor entre -1 e 1
        double y = random_double() * 2.0 - 1.0;  // Valor entre -1 e 1

        // Verificar se o ponto está dentro do círculo unitário
        if (x * x + y * y <= 1.0) {
            pontos_no_circulo++;
        }
    }

    // Estimar o valor de pi
    double pi_estimate = 4.0 * pontos_no_circulo / N;

    // Fim da medição do tempo
    double end_time = omp_get_wtime();
    double time_spent = end_time - start_time;

    // Exibir os resultados
    std::cout << "Estimativa de Pi (paralelo): " << pi_estimate << std::endl;
    std::cout << "Tempo de execução (paralelo): " << time_spent << " segundos" << std::endl;

    return 0;
}
