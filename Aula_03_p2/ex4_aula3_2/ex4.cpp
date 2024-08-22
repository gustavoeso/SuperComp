#include <iostream>
#include <vector>

class Vector {
public:
    // Construtor para inicializar o vetor com um tamanho específico
    Vector(int tamanho) : tam(tamanho), capacidade(tamanho){
        dados = new int[capacidade];
    };

    // Destrutor para liberar a memoria alocada dinamicamente
    ~Vector(){
        delete[] dados;
    }
    
    // Método para inicializar todos os elementos do vetor com um valor específico
    void inicializa(int valor){
        for(int i = 0; i < tam; i++){
            dados[i] = valor;
        }
    }

    // Método para acessar elementos em uma posição específica (leitura, ou get)
    int get(int index) const{
        if (index >= 0 && index < tam){
            return dados[index];
        } else{
        throw std::out_of_range("Index out of range");
        }
    }

    // Método para modificar o elemento em uma posição específica (escrita, ou set)
    void set(int index, int valor){
        if  (index >= 0 && index < tam){
            dados[index] = valor;
        } else{
        throw std::out_of_range("Index out of range");
        }   
    }
    
    // Método para inserir um elemento dentro do vetor em uma posição específica
    void inserir(int index, int valor){
        if (index < 0 || index > tam){
            throw std::out_of_range("Index out of range");
        }
        if (tam >= capacidade){
            redimensiona(capacidade*2);
        }
        for (int i = tam; i > index; i--){
            dados[i] = dados[i-1];
        }
        dados[index] = valor;
        tam++;
    }
    
    // Método para remover um elemento do vetor em uma posição específica
    void remover(int index){
        if (index < 0 || index > tam){
            throw std::out_of_range("Index out of range");
        }
        for (int i = index; i < tam-1; i++){
            dados[i] = dados[i+1];
        }
        tam--;
    }
    
    // Método para retornar o tamanho do vetor
    void imprime() const{
        for (int i = 0; i < tam; ++i) {
            std::cout << dados[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    int* dados;
    int tam;
    int capacidade;

    // Método para redimensionar o array dinâmico quando necessário
    void redimensiona(int novaCapacidade){
        int* novoArray = new int[novaCapacidade];
        for (int i = 0; i < tam; ++i) {
            novoArray[i] = dados[i];
        }
        delete[] dados;
        dados = novoArray;
        capacidade = novaCapacidade;
    }
};

// Função principal
int main() {
    Vector vec(5);
    vec.inicializa(0);
    vec.imprime();

    vec.set(0, 10);
    vec.set(1, 20);
    vec.set(2, 30);
    vec.set(3, 40);
    vec.set(4, 50);
    vec.imprime();

    vec.inserir(2, 25); // Insere o valor 25 na posição 2
    vec.imprime();

    vec.remover(4); // Remove o elemento na posição 4
    vec.imprime();

    return 0;
}