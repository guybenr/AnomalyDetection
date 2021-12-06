//
// Created by Guy Ben Razon 209207364 and Adi Aviv 206962904

#include <math.h>
#include "anomaly_detection_util.h"

/*
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the average of the values in the array
 */
float avg(float* x, int size) {
    if(size == 0) {
        return 0;
    }
    float sum (0);
    for (int i(0) ; i < size ; ++i) {
        sum += x[i];
    }
    return sum / (float)size;
}

/*
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the variance of the values in the array
 */
float var(float* x, int size) {
    if(size == 0) {
        return 0;
    }
    float sum(0);
    float average (avg(x,size));
    for (int i(0) ; i < size ; ++i) {
        sum += (x[i] - average) * (x[i] - average);
    }
    return sum / (float)size;
}

/*
 * param: float* y - an array of floats
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the covariance of the values in the array
 */
float cov(float* x,  float* y, int size) {
    if(size == 0) {
        return 0;
    }
    float sum(0);
    float avgX (avg(x,size));
    float avgY (avg(y,size));
    for (int i(0); i < size; ++i) {
        sum += (x[i] - avgX) * (y[i] - avgY);
    }
    return sum / (float)size;
}

/*
 * param: float* y - an array of floats
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the Pearson corrlation coefficient of X and Y
 */
float pearson(float* x, float* y, int size) {
    if(size == 0) {
        return 0;
    }
    float deviationX (sqrtf(var(x,size)));
    float deviationY (sqrtf(var(y,size)));
    return (cov(x,y,size) / (deviationX * deviationY));
}

/*
 * param: Point** - an array of points*
 * param: int size - the number of points
 * performs a linear regression and returns the line equation
 */
Line linear_reg(Point** points, int size) {
    float* x = new float [size];
    float* y = new float [size];
    for (int i(0) ; i < size ; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a (cov(x,y,size)/var(x,size));
    float b (avg(y,size) - (a * avg(x,size)));
    delete[] x;
    delete[] y;
    Line line(a,b);
    return line;
}

/**
 * param: Point p
 * param: Point** - an array of points*
 * param: int size - the number of points
 * returns the deviation between point p and the line equation of the points
 */
float dev(Point p,Point** points, int size) {
    Line line = linear_reg(points,size);
    return (fabsf(p.y - line.f(p.x)));
}

/*
 * param: Point p
 * param: Line l
 * returns the deviation between point p and the line
 */
float dev(Point p,Line l) {
    return (fabsf(p.y - l.f(p.x)));
}

float distance(Point a, Point b) {
    return sqrtf((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}


