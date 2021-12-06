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




//Circle MinCircleRecursive(vector<Point *> *points, vector<Point> *boundary) {
//    Circle *c = new Circle(Point(0, 0), 0);
//    bool flagBoundary = false, flagPoints = false;
//    Point p;
//    if (!points->empty()) {
//        p = *points->back();
//    }
//    if (points->empty() || boundary->size() == 3)
//        *c = *createCircle(boundary);
//    else {
//        flagPoints = true;
//        points->pop_back();
//        *c = MinCircleRecursive(points, boundary);
//        if (!c->isInCircle(p)) {
//            flagBoundary = true;
//            boundary->push_back(p);
//            *c = MinCircleRecursive(points, boundary);
//        }
//    }
//    if (flagBoundary && !boundary->empty())
//        boundary->pop_back();
//    if (flagPoints){
//        Point* pointer = &p;
//        points->push_back(pointer);
//    }
//    return *c;
//}
//
//Circle findMinCircle(Point **points, size_t size) {
//    vector<Point> boundary;
//    vector<Point *> ps(points, points + size);
//    Circle c = MinCircleRecursive(&ps, &boundary);
//    return c;
//}


//Circle* createCircle(vector<Point> points) {
//    if (points.empty()) {
//        Circle* newCircle = new Circle(Point(0, 0), 0);
//        return newCircle;
//    }
//    if (points.size() == 1) {
//        Circle* newCircle = new Circle(points.at(0), 0);
//        return newCircle;
//    }
//    if (points.size() == 2) {
//        Point a = points.at(0);
//        Point b = points.at(1);
//        double centerX = (a.x + b.x) / 2;
//        double centerY = (a.y + b.y) / 2;
//        double r = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) / 2;
//        Circle* newCircle = new Circle(Point(centerX, centerY), r);
//        return newCircle;
//    }
//    Point a = points.at(0);
//    Point b = points.at(1);
//    Point m = points.at(2);
//    double x1 = a.x;
//    double y1 = a.y;
//    double x2 = b.x;
//    double y2 = b.y;
//    double x3 = m.x;
//    double y3 = m.y;
//    double x12 = x1 - x2;
//    double x13 = x1 - x3;
//    double y12 = y1 - y2;
//    double y13 = y1 - y3;
//    double y31 = y3 - y1;
//    double y21 = y2 - y1;
//    double x31 = x3 - x1;
//    double x21 = x2 - x1;
//    // x1^2 - x3^2
//    double sx13 = powf(x1, 2) - powf(x3, 2);
//
//    // y1^2 - y3^2
//    double sy13 = powf(y1, 2) - powf(y3, 2);
//
//    double sx21 = powf(x2, 2) - powf(x1, 2);
//    double sy21 = powf(y2, 2) - powf(y1, 2);
//
//    double f = ((sx13) * (x12) + (sy13) * (x12) + (sx21) * (x13) + (sy21) * (x13)) / (2 * ((y31) * (x12) - (y21) * (x13)));
//    double g = ((sx13) * (y12) + (sy13) * (y12) + (sx21) * (y13) + (sy21) * (y13)) / (2 * ((x31) * (y12) - (x21) * (y13)));
//
//    double c = -powf(x1, 2) - powf(y1, 2) - 2 * g * x1 - 2 * f * y1;
//
//    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
//    // where centre is (h = -g, k = -f) and radius r
//    // as r^2 = h^2 + k^2 - c
//    double h = -g;
//    double k = -f;
//    double sqr_of_r = h * h + k * k - c;
//
//    // r is the radius
//    float r = sqrtf((float)sqr_of_r);
//
//    Circle* newCircle = new Circle(Point(h, k), sqrt(r));
//    if (isinf(r) || isinf(-r)) {
//        int x = 5;
//    }
//    return newCircle;
//}
