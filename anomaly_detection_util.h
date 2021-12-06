//
// Created by adida on 14/10/2021
//

#ifndef EX1_ANOMALY_DETECTION_UTIL_H
#define EX1_ANOMALY_DETECTION_UTIL_H

/*
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the average of the values in the array
 */
float avg(float* x, int size);

/*
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the variance of the values in the array
 */
float var(float* x, int size);

/*
 * param: float* y - an array of floats
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the covariance of the values in the array
 */
float cov(float* x,  float* y, int size);

/*
 * param: float* y - an array of floats
 * param: float* x - an array of floats
 * param: int size - the size of the array x
 * return: the Pearson corrlation coefficient of X and Y
 */
float pearson(float* x, float* y, int size);

// the class defines a line based on the equation y = ax + b
class Line{
public:
    float a,b;
    //constructors
    Line():a(0),b(0){}
    Line(float a, float b):a(a),b(b){}
    float f(float x){
        return a*x+b;
    }
};

// class defines a point based on it's x and y's values
class Point{
public:
    float x,y;

    //constructor
    Point(float x, float y):x(x),y(y){}
    Point():x(0),y(0){}
};//

/*
 * param: Point** - an array of points*
 * param: int size - the number of points
 * performs a linear regression and returns the line equation
 */
Line linear_reg(Point** points, int size);

/*
 * param: Point p
 * param: Point** - an array of points*
 * param: int size - the number of points
 * returns the deviation between point p and the line equation of the points
 */
float dev(Point p,Point** points, int size);

/*
 * param: Point p
 * param: Line l
 * returns the deviation between point p and the line
 */
float dev(Point p,Line l);

float distance(Point a, Point b);

#endif //EX1_ANOMALY_DETECTION_UTIL_H
