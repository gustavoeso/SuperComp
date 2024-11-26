#include <iostream>
#include <omp.h>

long long factorial(int n) {
    long long result = 1;

    #pragma omp parallel for reduction(*:result)
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }

    return result;
}

int main() {
    int num = 10;
    std::cout << "Fatorial de " << num << " é " << factorial(num) << std::endl;
    return 0;
}
