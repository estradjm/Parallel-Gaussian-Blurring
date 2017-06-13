#ifndef FILTER_H
#define FILTER_H

#define PI 3.14159265

typedef enum {
	FILTER_AVG, 
	FILTER_GAUSS
} filter_type;

typedef struct {
	int radius;
	double **matrix;
	int type;
} FILTER;

FILTER *filter_create_avg(int radius);

FILTER *filter_create_gauss(int radius, double sigma);

void filter_print(FILTER *filter);

void filter_free(FILTER *filter);

#endif /*FILTER_H*/
