#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=q3_1.txt          # Nome do arquivo de saída
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec

export OMP_NUM_THREADS=2

./q3