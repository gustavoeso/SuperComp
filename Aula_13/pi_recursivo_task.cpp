#include <omp.h>
#include <iostream>
#include <iomanip>
static long num_steps = 1024l*1024*1024*2;

#define MIN_BLK  1024*1024*1024

double pi_r(long Nstart, long Nfinish, double step) {
    long i, iblk;
    double sum = 0.0;

    if (Nfinish - Nstart < MIN_BLK) {
        // Caso base: somar diretamente sem criar uma nova tarefa
        double local_sum = 0.0;
        for (i = Nstart; i < Nfinish; i++) {
            double x = (i + 0.5) * step;
            local_sum += 4.0 / (1.0 + x * x);
        }
        sum = local_sum;
    } else {
        // Caso recursivo: dividir em duas subtarefas
        iblk = Nfinish - Nstart;
        double sum1 = 0.0, sum2 = 0.0;

        #pragma omp task shared(sum1)
        sum1 = pi_r(Nstart, Nfinish - iblk / 2, step);

        #pragma omp task shared(sum2)
        sum2 = pi_r(Nfinish - iblk / 2, Nfinish, step);

        #pragma omp taskwait
        sum = sum1 + sum2;
    }

    return sum;
}

int main() {
    double step, pi;
    double init_time, final_time;
    step = 1.0 / (double) num_steps;

    // Marca o tempo de início
    init_time = omp_get_wtime();

    double sum = 0.0;
    #pragma omp parallel
    {
        #pragma omp single
        sum = pi_r(0, num_steps, step);
    }

    // Calcula pi
    pi = step * sum;

    // Marca o tempo de fim e calcula a duração
    final_time = omp_get_wtime() - init_time;

    // Imprime o resultado e o tempo de execução
    std::cout << "For " << num_steps << " steps, pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";
}
