#include <vector>
#include <iostream>
#include <omp.h>
#include <unistd.h>

double conta_complexa(int i) {
    return 2 * i;
}

int main() {
    int N = 10000; 
    std::vector<double> vec;
    double start_time = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        #pragma omp critical
        {
            vec.push_back(conta_complexa(i));
        }
    }

    double end_time = omp_get_wtime();
    double duration = end_time - start_time;

    std::cout << "Tempo com omp critical: " << duration << " segundos.\n";

    return 0;
}
