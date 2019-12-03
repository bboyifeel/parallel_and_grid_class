# https://ulhpc-tutorials.readthedocs.io/en/latest/beginners/

# connect to the cluster
# ssh -p 8022 -i id_rsa ifilimonov@access-iris.uni.lu (without config)

ssh iris-cluster


# allocate resources
#-N  the number of distributed nodes you want to reserver with
#-n  o specify the total number of MPI processes you want
#--ntasks-per-node=  the number of MPI processes per node
srun -p interactive --qos qos-interactive --time 1:00:0 -N 2 -n 2 -c 4 --pty bash
#si -n 14


#OpenMP
gcc -fopenmp
#OR
icc -openmp


module purge
module load toolchain/intel
module load devel/Boost/


#MPI run and compile
mpiicpc 3_mpi.cpp -o 3_mpi -lpthread # for boost -lboost_mpi (instead)
srun -n $SLURM_NTASKS 3_mpi

#spark
srun -p interactive -N 2 -n 2 -c 28 -t 3:00:00 --pty bash
module load devel/Spark

start-master.sh --host $(hostname)
netstat -an | grep 80

sq
sjoin