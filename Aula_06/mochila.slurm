#!/bin/bash
#SBATCH --job-name=mochila                    # Nome do job
#SBATCH --output=mochila%j.txt               # Nome do arquivo de saída
#SBATCH --ntasks=1                              # Número de tarefas (4 processos)
#SBATCH --cpus-per-task=1                       # Número de threads por tarefa (2 threads por processo)
#SBATCH --mem=1024                               # Memória total alocada por nó (1024 MB = 1 GB)
#SBATCH --time=00:10:00                         # Tempo máximo de execução (10 minutos)
#SBATCH --partition=espec                      # fila do cluster a ser utilizada

# executa o executavel atv1

echo "problema da mochila"
echo " "
echo "entrada 1"
./mochila_in1
echo " "
echo "entrada 2"
./mochila_in2
echo " "
echo "entrada 3"
./mochila_in3
echo " "
echo "entrada 4"
./mochila_in4
echo " "
echo "30 linhas"
./mochila_grande