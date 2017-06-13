#ifndef BLUR_COMMON_H
#define BLUR_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../image.h"

void create_pixel_type(
	MPI_Datatype *type
);

void send_subimage(
	IMAGE *src, 
	MPI_Datatype *PIXEL, 
	int destination, 
	int start_index,
	int height
);

void recv_subimage(
	IMAGE *dest, 
	MPI_Datatype *PIXEL,
	int source, 
	int start_index, 
	int height
);

#endif /*BLUR_COMMON_H*/
