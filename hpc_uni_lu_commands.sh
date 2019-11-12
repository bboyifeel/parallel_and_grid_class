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
# icc -qopenmp -xhost -Wall -O2 src/a_2_3_openmp.cpp -o bin/a_2_3_openmp
# export OMP_NUM_THREADS=1
# ./a_2_3_openmp


module purge
module load toolchain/intel
module load devel/Boost/


#MPI run and compile
mpiicpc src/mpi_broadcast.cpp -o bin/mpi_broadcast -lpthread		//regular
mpiicpc src/a_2_2_boost_mpi.cpp -o bin/a_2_2_boost_mpi -lboost_mpi	//boost
srun -n $SLURM_NTASKS ~/assigments/bin/'your file'


#spark
srun -p interactive -N 2 -n 2 -c 28 -t 3:00:00 --pty bash
module load devel/Spark

start-master.sh --host $(hostname)
netstat -an | grep 80

sq
sjoin