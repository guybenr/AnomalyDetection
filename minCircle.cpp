//
// Created by guybe on 25/11/2
//

#include "minCircle.h"

#include "math.h"

//function return truee if p in is the circle and false otherwise
bool Circle::isInCircle(Point p) {
    return distance(this->center, p) <= this->radius;
}

Point get_circle_center(double bx, double by,
                        double cx, double cy)
{
    double B = bx * bx + by * by;
    double C = cx * cx + cy * cy;
    double D = bx * cy - by * cx;
    return Point((float) ((cy * B - by * C) / (2 * D)),
                 (float) ((bx * C - cx * B) / (2 * D)));
}

//creating a circle from three points
Circle circleFrom3Points(Point p1, Point p2, Point p3) {
    Point I = get_circle_center(p2.x - p1.x, p2.y - p1.y,
                                p3.x - p1.x, p3.y - p1.y);

    I.x += p1.x;
    I.y += p1.y;
    return Circle( I, distance(I, p1));
}

//creating a circle from two points
Circle circleFrom2Points(Point p1, Point p2) {
    Point p3 = Point((float)((p1.x + p2.x) / 2.0),  (float)((p1.y + p2.y) / 2.0));
    return Circle(p3, distance(p1, p2) / 2.0);
}

//method creates a minimum trivial circle based on either 0 - 3 points
Circle minTrivialCircle(vector<Point>& points) {
    if (points.empty()) {
        return Circle(Point(0, 0), 0);
    } else if (points.size() == 1) {
        return Circle(points[0], 0);
    } else if (points.size() == 2) {
        return circleFrom2Points(points[0], points[1]);
    }
    return circleFrom3Points(points[0], points[1], points[2]);
}

Circle MinCircleRecursive(vector<Point> &points, vector<Point> boundary, int size) {
    if (size == 0 || boundary.size() == 3) {
        return minTrivialCircle(boundary);
    }
    // randomly picking a point
    int i = rand() % size;
    Point p = points[i];
    swap(points[i], points[size - 1]);
    Circle c = MinCircleRecursive(points, boundary, size - 1);
    // if the point is in the circle then done
    if (c.isInCircle(p))
        return c;
    // else p is on the boundary
    boundary.push_back(p);
    return MinCircleRecursive(points, boundary, size - 1);
}

// method return the minimum circle that all of the points are in it' based on welzl algorithm
Circle findMinCircle(Point **points, size_t size) {
    vector<Point> boundary;
    vector<Point> ps;
    for (int i = 0; i < size; ++i) {
       ps.push_back(*points[i]);
    }
    return MinCircleRecursive(ps, boundary, size);
}

