#include <iostream>
#include <vector>
#include <chrono>

// Função Fibonacci com memoization
long long int fib(int n, std::vector<long long int>& memo) {
    if (n <= 1) {
        return n;
    }
    if (memo[n] != -1) {
        return memo[n]; // Retorna o valor já calculado
    }
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo); // Calcula e armazena o valor
    return memo[n];
}

int main() {
    int n;
    std::cout << "Digite o valor de N: ";
    std::cin >> n;

    // Inicializa o vetor de memoization com -1 para indicar valores não calculados
    std::vector<long long int> memo(n + 1, -1);

    // Medir o tempo de execução
    auto start = std::chrono::high_resolution_clock::now();

    long long int result = fib(n, memo);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Fibonacci de " << n << " é " << result << std::endl;
    std::cout << "Tempo de execução: " << duration.count() << " segundos" << std::endl;

    return 0;
}
