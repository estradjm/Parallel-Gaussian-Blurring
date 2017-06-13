#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

IMAGE *image_load(const char *image_name) {
	// Declare image structure
	IMAGE *image = (IMAGE*) malloc( sizeof(IMAGE) );

	// Open file
	FILE *file = fopen(image_name, "r");
	if(!file)
		return NULL;

	// Read image info
	fscanf(file, "%s", image->header);
	fscanf(file, "%d %d %d", &(image->width), &(image->height), &(image->color_depth));

	// Alocate memory for pixels
	image->pixels = (pixel**) malloc(image->height * sizeof(pixel*));
	int i, j;
	for(i = 0; i < image->height; i++)
		image->pixels[i] = (pixel*) malloc(image->width * sizeof(pixel));

	// Read pixels
	for(i = 0; i < image->height; i++)
		for(j = 0; j < image->width; j++)
			fscanf(file, "%c%c%c", &(image->pixels[i][j].R), &(image->pixels[i][j].G), &(image->pixels[i][j].B));

	// Close file
	fclose(file);

	return image;
}

int image_write(IMAGE *image, const char *file_name) {
	// Open file
	FILE *file = fopen(file_name, "w");
	if(!file)
		return 0;
	
	// Write image info
	fprintf(file, "%s\n%d %d\n%d", image->header, image->width, image->height, image->color_depth);

	// Write pixels
	int i, j;
	for(i = 0; i < image->height; i++)
		for(j = 0; j < image->width; j++)
			fprintf(file, "%c%c%c", image->pixels[i][j].R, image->pixels[i][j].G, image->pixels[i][j].B);

	// Write EOF
	fprintf(file, "%d", EOF);

	// Close file
	fclose(file);

	return 1;
}

IMAGE *image_create_blank(IMAGE *source) {
	// Declare
	IMAGE *image = (IMAGE*) malloc( sizeof(IMAGE) );

	//Copy info(except pixels)
	strcpy(image->header, source->header);
	image->height = source->height;
	image->width = source->width;
	image->color_depth = source->color_depth;

	// Allocate memory for pixels
	image->pixels = (pixel**) malloc(image->height * sizeof(pixel*));
	int i;
	for(i = 0; i < image->height; i++)
		image->pixels[i] = (pixel*) malloc(image->width * sizeof(pixel));

	return image;
}

void image_free(IMAGE *image) {
	// Free image pixels
	int i;
	for(i = 0; i < image->height; i++)
		free(image->pixels[i]);
	free(image->pixels);

	// Free image
	free(image);
}

// apply_to_pixel is a private function (local scope in image.c; used in apply_filer.c)
void apply_to_pixel(int x, int y, IMAGE *original, IMAGE *result, FILTER *filter) {
	if(x<filter->radius || y<filter->radius || x>=original->width-filter->radius || y>=original->height-filter->radius) {
		result->pixels[y][x] = original->pixels[y][x];
		return;
	}

	int i, j;
	pixel res;
	res.R = res.G = res.B = 0;
	double fil;

	for(i = -filter->radius; i <= filter->radius; i++) 
		for(j = -filter->radius; j <= filter->radius; j++) {
			fil = filter->matrix[i+filter->radius][j+filter->radius];
			res.R += fil * original->pixels[y+i][x+j].R;
			res.G += fil * original->pixels[y+i][x+j].G;
			res.B += fil * original->pixels[y+i][x+j].B;
		}
	
	result->pixels[y][x].R = res.R;
	result->pixels[y][x].G = res.G;
	result->pixels[y][x].B = res.B;
}

IMAGE *apply_filter(IMAGE *original, FILTER *filter) {
	IMAGE *result = image_create_blank(original);

	int x, y;
	for(y = 0; y < original->height; y++)
		for(x = 0; x < original->width; x++)
			apply_to_pixel(x, y, original, result, filter);

	return result;
}
