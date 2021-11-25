//
// Created by guybe on 25/11/2021.
//

#include "minCircle.h"
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

Circle MinCircleRecursive(vector<Point*>& points, vector<Point> boundary) {
    if (points.empty() || boundary.size() == 3)
        Circle c = createCircle(boundary);
    else {
        Point p = *points.end();

    }
}

Circle findMinCircle(Point** points,size_t size) {
    vector<Point> boundary;
    vector<Point*> ps(points, points + size);
    Circle c = MinCircleRecursive(ps,boundary);
}