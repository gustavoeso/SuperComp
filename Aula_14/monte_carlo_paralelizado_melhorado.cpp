#include <iostream>
#include <random>
#include <cmath>
#include <ctime>
#include <omp.h>

int main() {
    int N = 100000;  // Número de pontos
    int pontos_no_circulo = 0;
    
    // Início da medição do tempo
    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        // Gerador de números aleatórios específico para cada thread
        std::random_device rd;
        std::mt19937 generator(rd() + omp_get_thread_num());
        std::uniform_real_distribution<double> distribution(-1.0, 1.0);

        int pontos_thread = 0;  // Contador de pontos dentro do círculo para cada thread

        #pragma omp for
        for (int i = 0; i < N; i++) {
            // Gerar pontos (x, y) aleatórios no quadrado [-1, 1] x [-1, 1]
            double x = distribution(generator);
            double y = distribution(generator);

            // Verificar se o ponto está dentro do círculo unitário
            if (x * x + y * y <= 1.0) {
                pontos_thread++;
            }
        }

        // Redução para somar os pontos de todas as threads
        #pragma omp atomic
        pontos_no_circulo += pontos_thread;
    }

    // Estimar o valor de pi
    double pi_estimate = 4.0 * pontos_no_circulo / N;

    // Fim da medição do tempo
    double end_time = omp_get_wtime();
    double time_spent = end_time - start_time;

    // Exibir os resultados
    std::cout << "Estimativa de Pi (melhorada paralela): " << pi_estimate << std::endl;
    std::cout << "Tempo de execução (melhorada paralela): " << time_spent << " segundos" << std::endl;

    return 0;
}
