#ifndef BLUR_COMMON_H
#define BLUR_COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "metadata.h"
#include "../image.h"

void create_pixel_type(
	MPI_Datatype *type
);

#endif /*BLUR_COMMON_H*/
