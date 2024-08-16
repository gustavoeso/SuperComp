//Manipulação de Strings: Palíndromo: Escreva um programa que verifique se uma string inserida pelo usuário é um palíndromo (lê-se da mesma forma de trás para frente).

#include <iostream>

int main(){
    std::string str;
    std::cout << "Digite uma string: ";
    std::cin >> str;

    bool palindromo = true;
    for (int i = 0; i < str.size()/2; i++){
        if(str[i] != str[str.size()-1-i]){
            palindromo = false;
            break;
        }
    }

    if(palindromo){
        std::cout << "A string é um palíndromo" << std::endl;
    }
    else{
        std::cout << "A string não é um palíndromo" << std::endl;
    }

    return 0;
}