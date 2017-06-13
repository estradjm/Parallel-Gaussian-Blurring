#include <mpi.h>
#include <stdio.h>
#include "master.h"
#include "slave.h"

int main(int argc, char *argv[]) {
	int world_size, world_rank;

	//Init
  	MPI_Init(NULL, NULL);
  
  	//Get number of procs
  	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  
  	//Proc rank
  	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if(world_rank == 0) //master
		master_run(world_size);
	else //slave
		slave_run(world_rank);

	//End MPI
	MPI_Finalize();
}

