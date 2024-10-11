#!/bin/bash
#SBATCH --job-name=vetor_insert_pre_alocacao_test
#SBATCH --output=output_vetor_insert_pre_alocacao.txt
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem-per-cpu=1Gb
#SBATCH --time=00:05:00

./vetor_insert_pre_alocacao