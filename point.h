#ifndef __POINT_H
#define __POINT_H

#include "header.h"

typedef struct {
	
	double x, y, z;
} point_t;

double degreToRadian(double);
double radianToDegre(double);
point_t initPoint(double, double, double);
point_t addPoint(point_t, point_t);
point_t subPoint(point_t, point_t);
point_t mulPoint(point_t, double);
point_t oppPoint(point_t);
int isEgalPoint(point_t, point_t);
int isOppPoint(point_t, point_t);
double distance(point_t, point_t);
double angle(point_t, point_t, point_t);
point_t middle(point_t, point_t);
point_t normalisation(point_t, double);
point_t plan(point_t, point_t, point_t);
point_t rotation(point_t, double, point_t);
void getRotation(point_t, point_t, double*, point_t*);
double getAngleRotation(point_t, point_t, point_t);
void writePoint(point_t);

#endif
