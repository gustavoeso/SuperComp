#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=exercicios.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=5
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=10

echo "Questao 4"
mpirun -np 4 ./q4

echo " "

echo "Questao 4 teste com matriz fixa para comparar o resultado e ver se ele está fazendo o cáculo correto"
mpirun -np 4 ./q4_teste