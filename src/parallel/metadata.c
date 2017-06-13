#include "metadata.h"

void meta_init(METADATA *meta, int width, int height, filter_type type, int radius, double sigma)
{
        //Set info
        meta->width = width;
        meta->height = height;
	meta->type = type;
        meta->radius = radius;
        meta->sigma = sigma;
}

void meta_print(METADATA *meta)
{
	printf("Metadata:\n");
	printf("\t->width=%d\n", meta->width);
	printf("\t->height=%d\n", meta->height);
	printf("\t->type=%d\n", meta->type);
	printf("\t->radius=%d\n", meta->radius);
	printf("\t->sigma=%lf\n", meta->sigma);
}

void meta_create_type(MPI_Datatype *type)
{
	//Get int extent
	MPI_Aint extent;
	MPI_Type_extent(MPI_INT, &extent);

	//Prepare params
	int count = 2;
	int blocklengths[2] = { 4, 1 };
	int offsets[2] = { 0, blocklengths[0]*extent};
	MPI_Datatype oldtypes[2] = { MPI_INT, MPI_DOUBLE };

	//Create type
	MPI_Type_struct(count, blocklengths, offsets, oldtypes, type);

	//Commit type
	MPI_Type_commit(type);
}
