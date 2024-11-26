#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=exercicios.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=5
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=10

echo "Parte A"
./parteA

echo " "

echo "Parte B"
./parteB

echo " "

echo "Parte C"
./parteC

echo " "

echo "Parte D"
./parteD