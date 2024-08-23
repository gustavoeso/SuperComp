#!/bin/bash
#SBATCH --job-name=Atividade                    # Nome do job
#SBATCH --output=atividade%j.txt               # Nome do arquivo de saída
#SBATCH --ntasks=4                              # Número de tarefas (4 processos)
#SBATCH --cpus-per-task=4                       # Número de threads por tarefa (2 threads por processo)
#SBATCH --mem=1024                               # Memória total alocada por nó (1024 MB = 1 GB)
#SBATCH --time=00:10:00                         # Tempo máximo de execução (10 minutos)
#SBATCH --partition=espec                      # fila do cluster a ser utilizada

# executa o executavel atv1

echo "Gprof"
./convolucao_gprof