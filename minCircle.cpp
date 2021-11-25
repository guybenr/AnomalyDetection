//
// Created by guybe on 25/11/2021.
//

#include "minCircle.h"
#include "anomaly_detection_util.h"

Circle* createCircle(vector<Point> *points) {
    if (points->empty()) {
        Circle newCircle(Point(0, 0), 0);
        return &newCircle;
    }
    if (points->size() == 1) {
        Circle newCircle(points->at(0), 0);
        return &newCircle;
    }
    if (points->size() == 2) {
        Point a = points->at(0);
        Point b = points->at(1);
        float centerX = (a.x + b.x) / 2;
        float centerY = (a.y + b.y) / 2;
        float r = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
        Circle newCircle(Point(centerX, centerY), r);
        return &newCircle;
    }
    Point a = points->at(0);
    Point b = points->at(1);
    Point c = points->at(2);
    float xab = a.x - b.x;
    float xac = a.x - c.x;
    float xca = c.x - a.x;
    float xba = b.x - a.x;
    float yab = a.y - b.y;
    float yac = a.y - c.y;
    float yca = c.y - a.y;
    float yba = b.y - a.y;
    float pXac = (a.x * a.x) - (c.x * c.x);
    float pXba = (b.x * b.x) - (a.x * a.x);
    float pYac = (a.y * a.y) - (c.y * c.y);
    float pYba = (b.y * b.y) - (a.y * a.y);
    float f =
            ((pXac) * (xab) + (pYac) * (xab) + (pXba) * (xac) + (pYba) * (xac)) / (2 * ((yca) * (xab) - (yba) * (xac)));
    float g =
            ((pXac) * (yab) + (pYac) * (yab) + (pXba) * (yac) + (pYba) * (yac)) / (2 * ((xca) * (yab) - (xba) * (yac)));
    // center of circle = (-g,-f)
    float k = -(a.x * a.x) - (a.y * a.y) - 2 * g * a.x - 2 * f * a.y;
    float radius = (g * g) + (f * f) - k;
    // radius of circle = sqrt radius
    Circle newCircle(Point(-g, -f), sqrt(radius));
    return &newCircle;
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