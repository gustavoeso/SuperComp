#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=q3_8_cpu.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=8

echo "Questao 3"
./q3