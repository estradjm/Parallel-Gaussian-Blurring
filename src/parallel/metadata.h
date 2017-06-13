#ifndef METADATA_H
#define METADATA_H

#include <stdio.h>
#include <mpi.h>
#include "../filter.h"

typedef struct {
	//Image info
	int width;
	int height;
	
	//Filter info
	int type;
	int radius;
	double sigma;
} METADATA;

//Sets the corresponding params to the METADATA structure
void meta_init(
	METADATA *meta, 
	int width, 
	int height, 
	filter_type type, 
	int radius, 
	double sigma
);

void meta_print(
	METADATA *meta
);

void meta_create_type(
	MPI_Datatype *type
);

#endif /*METADATA_H*/
