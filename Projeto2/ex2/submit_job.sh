#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=exercicios.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec


echo "exercicio 2 - hibrido (openMP + MPI)"
mpirun -np 4 ./ex2_hibrido