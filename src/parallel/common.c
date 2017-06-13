#include "common.h"

void create_pixel_type(MPI_Datatype *type)
{
	//Prepare params
	int count = 1;
	int blocklengths = 3;
	int offsets = 0;
	MPI_Datatype oldtypes = MPI_CHAR;

	//Create type
	MPI_Type_struct(count, &blocklengths, &offsets, &oldtypes, type);

	//Commit type
	MPI_Type_commit(type);
}

void send_subimage(IMAGE *src, MPI_Datatype *PIXEL, int destination, int start_index, int height) {
	//Iterator
	int i;

	//Send rows
	for(i = 0; i < height; i++)
		MPI_Send((src->pixels)[start_index+i], src->width, *PIXEL, destination, 0, MPI_COMM_WORLD);
}

void recv_subimage(IMAGE *dest, MPI_Datatype *PIXEL,int source, int start_index, int height)
{
	//Iterator
	int i;

	//Recv rows
	for(i = 0; i < height; i++)
		MPI_Recv((dest->pixels)[start_index+i], dest->width, *PIXEL, source, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}
