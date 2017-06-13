#include "master.h"


void master_run(int world_size) 
{
	//Info to be read
	char image_src_name[IMAGE_NAME_LENGHT], image_dest_name[IMAGE_NAME_LENGHT];
	int radius, type;
	double sigma;

	//Read info
	printf("Source image name: "); scanf("%s", image_src_name);
	printf("Destination image name: "); scanf("%s", image_dest_name);
	printf("Radius: "); scanf("%d", &radius);
	printf("Type: "); scanf("%d", &type);
	printf("Sigma: "); scanf("%lf", &sigma);

	//Load image from file
	printf("Loading image \"%s\"...\n", image_src_name);
	IMAGE *src = image_load(image_src_name);

	//Send metadata to the slaves
	printf("Sending metadata...\n");
	master_send_metadata(src, type, radius, sigma, world_size-1);

	//Send pixels
	printf("Sending data to slaves...\n");
	master_send_pixels(src, radius, world_size-1);

	//Recv image
	printf("Working...\n");
	IMAGE *result = master_recv_blur(src, radius, world_size-1);

	//Write image to disk
	printf("Writing image to disk...\n");
	image_write(result, image_dest_name);

	//Free memory
	image_free(src);
	image_free(result);

	//Master finished the work
	printf("DONE!\n");
}

int master_send_metadata(IMAGE *image, int type, int radius, double sigma, int n_slaves)
{
	//Init metadata
	METADATA meta;
	meta_init(&meta, image->width, image->height, type, radius, sigma);
	
	//Prepare metadata mpi datatype
	MPI_Datatype META;
	meta_create_type(&META);
	
	//Calculate the total data size(ignoring the endges)
	int data_size = image->height - 2*radius;
	int i;
	const int max_size = image->height / n_slaves;

	//Update and send the metadata
	for(i = 0; i < n_slaves && data_size != 0; i++) {
		master_update_metadata(&meta, &data_size, max_size);
		MPI_Send(&meta, 1, META, i+1, 0 , MPI_COMM_WORLD);
	}

	return i+1;
}

int master_update_metadata(METADATA *meta, int *data_size, int max_size)
{
	if(*data_size == 0) {
		meta->height = 0;
		return 0;
	}

	//Calculate send data lenght and update remaining data
	int actual_data = max_size;
	if(actual_data > *data_size) {
		actual_data = *data_size;
		*data_size = 0;
	} else {
		(*data_size) -= actual_data;
	}

	//The total data length to be sent
	int send_data = actual_data + 2*meta->radius;

	//Update metadata
	meta->height = send_data;

	return *data_size;
}

void master_send_pixels(IMAGE *src, int radius, int n_slaves)
{
	//Init metadata
	METADATA meta;
	meta_init(&meta, src->width, src->height, 0, radius, 0);

	//Prepare pixel type
	MPI_Datatype PIXEL;
	create_pixel_type(&PIXEL);

	//Calculate the total data size(ignoring the endges)
	int data_size = src->height - 2*meta.radius;
	int i, index = 0;
	const int max_size = src->height / n_slaves;


	//Update and send the metadata
	for(i = 0; i < n_slaves && data_size != 0; i++) {
		//Update metadata
		master_update_metadata(&meta, &data_size, max_size);
		
		//Send data
		send_subimage(src, &PIXEL, i+1, index, meta.height);

		//Update index
		index += meta.height - 2*meta.radius;
	}
}

IMAGE *master_recv_blur(IMAGE *src, int radius, int n_slaves) 
{
	//Allocate mem for the new image
	IMAGE *result = image_create_blank(src);

	//Copy first and last "radius" rows
	int i, j;
	for(i = 0; i < radius; i++) 
		for(j = 0; j < src->width; j++) {
			(result->pixels)[i][j] = (src->pixels)[i][j];
			(result->pixels)[src->height-1-i][j] = (src->pixels)[src->height-1-i][j];
		}

	//Init metadata
	METADATA meta;
	meta_init(&meta, src->width, src->height, 0, radius, 0);

	//Prepare pixel type
	MPI_Datatype PIXEL;
	create_pixel_type(&PIXEL);

	//Calculate the total data size(ignoring the endges)
	int data_size = src->height - 2*meta.radius;
	int index = 0;
	const int max_size = src->height / n_slaves;

	//Update and send the metadata
	for(i = 0; i < n_slaves && data_size != 0; i++) {
		//Update metadata
		master_update_metadata(&meta, &data_size, max_size);

		//Recv data
		recv_subimage(result, &PIXEL, i+1, index+radius, meta.height-radius);

		//Update index
		index += meta.height - 2*meta.radius;
	}

	return result;
}
