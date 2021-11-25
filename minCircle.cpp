//
// Created by guybe on 25/11/2021.
//

#include "minCircle.h"
#include "anomaly_detection_util.h"
#include "math.h"

Circle* createCircle(vector<Point> *points) {
    if (points->empty()) {
        Circle* newCircle = new Circle(Point(0, 0), 0);
        return newCircle;
    }
    if (points->size() == 1) {
        Circle* newCircle = new Circle(points->at(0), 0);
        return newCircle;
    }
    if (points->size() == 2) {
        Point a = points->at(0);
        Point b = points->at(1);
        float centerX = (a.x + b.x) / 2;
        float centerY = (a.y + b.y) / 2;
        float r = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)) / 2;
        Circle* newCircle = new Circle(Point(centerX, centerY), r);
        return newCircle;
    }
    Point a = points->at(0);
    Point b = points->at(1);
    Point m = points->at(2);
    float x1 = a.x;
    float y1 = a.y;
    float x2 = b.x;
    float y2 = b.y;
    float x3 = m.x;
    float y3 = m.y;
    float x12 = x1 - x2;
    float x13 = x1 - x3;
    float y12 = y1 - y2;
    float y13 = y1 - y3;
    float y31 = y3 - y1;
    float y21 = y2 - y1;
    float x31 = x3 - x1;
    float x21 = x2 - x1;

    // x1^2 - x3^2
    float sx13 = pow(x1, 2) - pow(x3, 2);

    // y1^2 - y3^2
    float sy13 = pow(y1, 2) - pow(y3, 2);

    float sx21 = pow(x2, 2) - pow(x1, 2);
    float sy21 = pow(y2, 2) - pow(y1, 2);

    float f = ((sx13) * (x12) + (sy13) * (x12) + (sx21) * (x13) + (sy21) * (x13)) / (2 * ((y31) * (x12) - (y21) * (x13)));
    float g = ((sx13) * (y12) + (sy13) * (y12) + (sx21) * (y13) + (sy21) * (y13)) / (2 * ((x31) * (y12) - (x21) * (y13)));

    float c = -pow(x1, 2) - pow(y1, 2) - 2 * g * x1 - 2 * f * y1;

    // eqn of circle be x^2 + y^2 + 2*g*x + 2*f*y + c = 0
    // where centre is (h = -g, k = -f) and radius r
    // as r^2 = h^2 + k^2 - c
    float h = -g;
    float k = -f;
    float sqr_of_r = h * h + k * k - c;

    // r is the radius
    float r = sqrt(sqr_of_r);

    Circle* newCircle = new Circle(Point(h, k), sqrt(r));
    return newCircle;
}

bool Circle::isInCircle(Point p) {
    float x = (p.x - center.x) * (p.x - center.x);
    float y = (p.y - center.y) * (p.y - center.y);
    if (x + y <= (this->radius * this->radius)) {
        return true;
    }
    return false;
}

Circle MinCircleRecursive(vector<Point *> *points, vector<Point> *boundary) {
    Circle *c = new Circle(Point(0, 0), 0);
    bool flagBoundary = false, flagPoints = false;
    Point p;
    if (!points->empty()) {
        p = *points->back();
    }
    if (points->empty() || boundary->size() == 3)
        c = createCircle(boundary);
    else {
        flagBoundary = true;
        points->pop_back();
        *c = MinCircleRecursive(points, boundary);
        if (!c->isInCircle(p)) {
            flagPoints = true;
            boundary->push_back(p);
            *c = MinCircleRecursive(points, boundary);
        }
    }
    if (flagBoundary && !boundary->empty())
        boundary->pop_back();
    if (flagPoints){
        Point* pointer = &p;
        points->push_back(pointer);
    }
    return *c;
}

Circle findMinCircle(Point **points, size_t size) {
    vector<Point> boundary;
    vector<Point *> ps(points, points + size);
    Circle c = MinCircleRecursive(&ps, &boundary);
    return c;
}