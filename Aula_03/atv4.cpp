#include <iostream>

int main(){
    std::string str;
    std::cout << "Digite uma string: ";
    std::cin >> str;

    std::cout << "A string tem " << str.size() << " caracteres" << std::endl;

    return 0;
}