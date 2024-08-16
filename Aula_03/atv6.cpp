#include <iostream>

int main(){
    int vetor[10];
    int maior_num = 0;

    for (int i = 0; i < 10; i++){
        std::cout << "Digite o " << i+1 << "º número: ";
        std::cin >> vetor[i];
        if (vetor[i] > maior_num){
            maior_num = vetor[i];
        }
    }
        std::cout << "O maior número é: " << maior_num << std::endl;

    return 0;
}