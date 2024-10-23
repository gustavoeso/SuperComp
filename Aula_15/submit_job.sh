#!/bin/bash
#SBATCH --job-name=exercicios          # Nome do job
#SBATCH --output=exercicios.txt          # Nome do arquivo de saída
#SBATCH --ntasks=4
#SBATCH --cpus-per-task=1
#SBATCH --mem=4096
#SBATCH --time=03:00:00
#SBATCH --partition=espec


# Executar o programa com 2 processos
echo "Exericio 1"
mpirun -np 2 ./ex1
echo " "
echo "Exercicio 2 com 4 processos"
mpirun -np 4 ./ex2
echo " "
echo "Exercicio 2 com 2 processos"
mpirun -np 2 ./ex2
echo " "
echo "Exercicio 3"
mpirun -np 4 ./ex3
echo " "
echo "Exercicio 4"
mpirun -np 4 ./ex4
echo " "
echo "Exercicio 5"
mpirun -np 4 ./ex5
echo " "
echo "Exercicio 6"
mpirun -np 4 ./ex6