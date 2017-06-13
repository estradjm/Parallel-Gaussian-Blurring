#include "slave.h"
#include <string.h>

void slave_run(int world_rank)
{
	int i, j;
	//Recv metadata from master
	MPI_Datatype META;
	meta_create_type(&META);
	METADATA meta;
	MPI_Recv(&meta, 1, META, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	//Stop process if there is no data
	if(meta.height == 0)
		return;

	//Alocate memory for the subimage
	IMAGE *sub_image = (IMAGE*) malloc(sizeof(IMAGE));
	sub_image->pixels = (pixel**) malloc(meta.height * sizeof(pixel*));
	for(i = 0; i < meta.height; i++)
		(sub_image->pixels)[i] = (pixel*) malloc(meta.width * sizeof(pixel));
	
	//Extract into the subimage the width and height from the metadata
	sub_image->width = meta.width;
	sub_image->height = meta.height;

	//Recv pixels
	MPI_Datatype PIXEL;
	create_pixel_type(&PIXEL);
	recv_subimage(sub_image, &PIXEL, 0, 0, meta.height);

	//Create filter	TODO: use meta.type to select filter
	FILTER *filter = filter_create_gauss(meta.radius, meta.sigma);

	//Apply filter
	IMAGE *result = apply_filter(sub_image, filter);

	//Send data back to master
	send_subimage(result, &PIXEL, 0, meta.radius, meta.height-meta.radius);

	//Free memory
	image_free(sub_image);
	image_free(result);
	filter_free(filter);
}
