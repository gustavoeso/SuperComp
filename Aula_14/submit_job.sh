#!/bin/bash
#SBATCH --job-name=paralelizacao_test
#SBATCH --output=output_paralelizacao_melhorado.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1Gb
#SBATCH --time=00:05:00

./monte_carlo_paralelizado_melhorado