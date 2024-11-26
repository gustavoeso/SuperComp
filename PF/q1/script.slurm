#!/bin/bash
#SBATCH --job-name=job_prova           # Nome do job
#SBATCH --output=exercicios.txt        # Nome do arquivo de saída
#SBATCH --nodes=5                      # Solicita 2 máquinas (nós)
#SBATCH --ntasks-per-node=1            # Solicita 5 tarefas (cores) por nó
#SBATCH --cpus-per-task=4              # CPUs por task
#SBATCH --mem=16G                     # Memória solicitada por nó (pode ser escrito como 3G)
#SBATCH --time=01:00:00
#SBATCH --partition=prova

export OMP_NUM_THREADS=15

./programa