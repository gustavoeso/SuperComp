#!/bin/bash
#SBATCH --job-name=mochila                    # Nome do job
#SBATCH --output=mochila%j.txt               # Nome do arquivo de saída
#SBATCH --ntasks=1                              # Número de tarefas (4 processos)
#SBATCH --cpus-per-task=1                       # Número de threads por tarefa (2 threads por processo)
#SBATCH --mem=4096                               # Memória total alocada por nó (1024 MB = 1 GB)
#SBATCH --time=02:00:00                         # Tempo máximo de execução (10 minutos)
#SBATCH --partition=espec                      # fila do cluster a ser utilizada

# executa o executavel atv1

echo "problema da mochila"
echo " "
echo "Entrada 1"
echo " "
./mochila Entrada1.txt
echo " "
echo "Entrada 2"
echo " "
./mochila Entrada2.txt
echo " "
echo "Entrada 4"
echo " "
./mochila Entrada4.txt
echo " "
echo "Entrada 3"
echo " "
./mochila Entrada3.txt
echo " "
