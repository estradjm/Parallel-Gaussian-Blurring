#ifndef IMAGE_H
#define IMAGE_H

#include "filter.h"

typedef struct {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} pixel;

//.ppm image
typedef struct {
	char header[3];
	int width, height;
	int color_depth;
	pixel **pixels;
} IMAGE;

//Loads an .ppm image from a given file
IMAGE *image_load(const char *image_name);

//Writes the given image to the "image->image_name" file
int image_write(IMAGE *image, const char *file_name);

//
IMAGE *image_create_blank(IMAGE *source);

//Free
void image_free(IMAGE *image);

//Apply a filter to the image
IMAGE *apply_filter(IMAGE *original, FILTER *filter);

#endif /*IMAGE_H*/
