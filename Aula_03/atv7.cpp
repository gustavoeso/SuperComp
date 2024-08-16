#include <iostream>

int main(){
    int vetor[10];
    int aux;

    for (int i = 0; i < 10; i++){
        std::cout << "Digite o " << i+1 << "º número: ";
        std::cin >> vetor[i];
    }

    for (int i = 0; i < 10; i++){
        for (int j = 0; j < 10; j++){
            if (vetor[i] < vetor[j]){
                aux = vetor[i];
                vetor[i] = vetor[j];
                vetor[j] = aux;
            }
        }
    }

    std::cout << "Vetor ordenado: ";
    for (int i = 0; i < 10; i++){
        std::cout << vetor[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}