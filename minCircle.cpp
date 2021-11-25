//
// Created by guybe on 25/11/2021.
//

#include "minCircle.h"

Circle createCircle(vector<Point> points) {
    Point a = points[0];
    Point b = points[1];
    Point c = points[2];
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
    float f = ((pXac) * (xab) + (pYac) * (xab) + (pXba) * (xac) + (pYba) * (xac)) / (2 * ((yca) * (xab) - (yba) * (xac)));
    float g = ((pXac) * (yab) + (pYac) * (yab) + (pXba) * (yac) + (pYba) * (yac)) / (2 * ((xca) * (yab) - (xba) * (yac)));
    // center of circle = (-g,-f)
    float k = -(a.x * a.x) - (a.y * a.y) - 2 * g * a.x - 2 * f * a.y;
    float radius = (g * g) + (f * f) - k;
    // radius of circle = sqrt radius
}
#include "math.h"
#include "anomaly_detection_util.h"
bool Circle:: isInCircle(Point p) {
    float x = (p.x - center.x) * (p.x - center.x);
    float y = (p.y - center.y) * (p.y - center.y);
    if (x + y <= (this->radius * this->radius)) {
        return true;
    }
    return false;
}

Circle MinCircleRecursive(vector<Point*> points, vector<Point> boundary) {
    Circle* c = new Circle(Point(0, 0), 0);
    if (points.empty() || boundary.size() == 3)
        *c = createCircle(boundary);
    else {
        Point p = **points.end();
        points.pop_back();
        *c = MinCircleRecursive(points, boundary);
        if (!c->isInCircle(p)) {
            boundary.push_back(p);
            *c = MinCircleRecursive(points, boundary);
        }
        return *c;
    }
}

Circle findMinCircle(Point** points,size_t size) {
    vector<Point> boundary;
    vector<Point*> ps(points, points + size);
    Circle c = MinCircleRecursive(ps,boundary);
}