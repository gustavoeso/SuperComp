#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=exercicios.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec


echo "exercicio 1 - openMP"
./exercicio1_openmp
echo " "
echo "exercicio 1 - MPI"
mpirun -np 4 ./exercicio1_mpi
echo " "
echo "exercicio 1 - hibrido (openMP + MPI)"
mpirun -np 4 ./ex1_hibrido