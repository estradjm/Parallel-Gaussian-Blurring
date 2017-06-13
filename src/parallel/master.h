#ifndef BLUR_MASTER_H
#define BLUR_MASTER_H

#include "common.h"
#include "metadata.h"

#define IMAGE_NAME_LENGHT 50

void master_run(
	int world_size
);

//Send the metadata for every process, return the actual number of slaves that will be used
int master_send_metadata(
	IMAGE *image,
	int type,  
	int radius, 
	double sigma, 
	int n_slaves
);

//TODO: exit if size > n_slaves/2
int master_update_metadata(
	METADATA *meta, 
	int *data_size, 
	int max_size
);

void master_send_pixels(
	IMAGE *src, 
	int radius, 
	int n_slaves
);

IMAGE *master_recv_blur(
	IMAGE *src,
	int radius, 
	int n_slaves
);

#endif /*BLUR_MASTER_H*/
