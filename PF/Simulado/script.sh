#!/bin/bash
#SBATCH --job-name=job_prova           # Nome do job
#SBATCH --output=exercicios.txt        # Nome do arquivo de saída
#SBATCH --nodes=2                      # Solicita 2 máquinas (nós)
#SBATCH --ntasks-per-node=5            # Solicita 5 tarefas (cores) por nó
#SBATCH --mem=3072                     # Memória solicitada por nó (pode ser escrito como 3G)
#SBATCH --time=03:00:00
#SBATCH --partition=prova

export OMP_NUM_THREADS=10

./executavel