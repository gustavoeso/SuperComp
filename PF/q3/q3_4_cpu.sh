#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=q3_4_cpu.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=4

echo "Questao 3"
./q3