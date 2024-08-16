#include <iostream>

// Matriz e Soma de Elementos Diagonais: Crie um programa que peça ao usuário para inserir uma matriz 3x3 e depois exiba a soma dos elementos na diagonal principal.

int main(){
    int matriz[3][3];
    int soma = 0;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            std::cout << "Digite o elemento da linha " << i+1 << " e coluna " << j+1 << ": ";
            std::cin >> matriz[i][j];
            if(i == j){
                soma += matriz[i][j];
            }
        }
    }

    std::cout << "A soma dos elementos da diagonal principal é: " << soma << std::endl;

    return 0;
}