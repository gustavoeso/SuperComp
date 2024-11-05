#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=exercicios.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec


echo "Exercicio 1"
mpirun -np 4 ./exercicio1
echo " "
echo "Exercicio 3"
mpirun -np 4 ./exercicio3
echo " "
echo "Exercicio 4"
mpirun -np 4 ./exercicio4