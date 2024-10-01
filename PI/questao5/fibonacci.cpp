#include <iostream>
#include <chrono>  // Para medir o tempo de execução

// Função Fibonacci recursiva simples (sem memoization)
long long int fib(int n) {
    if (n <= 1) {
        return n;
    } else {
        return fib(n - 1) + fib(n - 2);
    }
}

int main() {
    int n;
    std::cout << "Digite o valor de N: ";
    std::cin >> n;

    // Medir o tempo de execução
    auto start = std::chrono::high_resolution_clock::now();

    long long int result = fib(n);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Fibonacci de " << n << " é " << result << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
