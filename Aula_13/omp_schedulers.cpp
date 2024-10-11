#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

// Função para medir o tempo de execução
double measure_time(void (*function)(std::vector<std::vector<int>>& vectors, int n), 
                    const std::string& description, 
                    int n) {
    std::vector<std::vector<int>> vectors(4, std::vector<int>());
    double start_time = omp_get_wtime();
    function(vectors, n);
    double end_time = omp_get_wtime();
    double duration = end_time - start_time;

    std::cout << description << " took " << duration << " seconds.\n";
    return duration;
}

// Funções de agendamento
void scheduleDefault(std::vector<std::vector<int>>& vectors, int n) {
    #pragma omp parallel num_threads(4) shared(vectors, n)
    {
        #pragma omp for
        for (int i = 0; i < n; i++) {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleStatic(std::vector<std::vector<int>>& vectors, int n) {
    #pragma omp parallel num_threads(4) shared(vectors, n)
    {
        #pragma omp for schedule(static)
        for (int i = 0; i < n; i++) {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleDynamic(std::vector<std::vector<int>>& vectors, int n) {
    #pragma omp parallel num_threads(4) shared(vectors, n)
    {
        #pragma omp for schedule(dynamic)
        for (int i = 0; i < n; i++) {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleGuided(std::vector<std::vector<int>>& vectors, int n) {
    #pragma omp parallel num_threads(4) shared(vectors, n)
    {
        #pragma omp for schedule(guided)
        for (int i = 0; i < n; i++) {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

void scheduleAuto(std::vector<std::vector<int>>& vectors, int n) {
    #pragma omp parallel num_threads(4) shared(vectors, n)
    {
        #pragma omp for schedule(auto)
        for (int i = 0; i < n; i++) {
            vectors[omp_get_thread_num()].push_back(i);
        }
    }
}

int main() {
    const int n = 64;

    // Realiza 3 execuções para cada tipo de agendamento e calcula a média
    double time_default = 0.0;
    double time_static = 0.0;
    double time_dynamic = 0.0;
    double time_guided = 0.0;
    double time_auto = 0.0;

    for (int i = 0; i < 3; i++) {
        time_default += measure_time(scheduleDefault, "default", n);
        time_static += measure_time(scheduleStatic, "schedule(static)", n);
        time_dynamic += measure_time(scheduleDynamic, "schedule(dynamic)", n);
        time_guided += measure_time(scheduleGuided, "schedule(guided)", n);
        time_auto += measure_time(scheduleAuto, "schedule(auto)", n);
    }

    std::cout << "\nAverage execution times (3 runs):\n";
    std::cout << "default:               " << time_default / 3 << " seconds.\n";
    std::cout << "schedule(static):      " << time_static / 3 << " seconds.\n";
    std::cout << "schedule(dynamic):     " << time_dynamic / 3 << " seconds.\n";
    std::cout << "schedule(guided):      " << time_guided / 3 << " seconds.\n";
    std::cout << "schedule(auto):        " << time_auto / 3 << " seconds.\n";

    return 0;
}
