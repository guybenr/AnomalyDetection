
#include "HybridAnomalyDetector.h"


HybridAnomalyDetector::~HybridAnomalyDetector() {
}

float HybridAnomalyDetector::corThreshold() {
    return 0.5;
}


float HybridAnomalyDetector::getThreshold(vector<float>& f1, vector<float>& f2, int featureSize, float cor) {
    //create the two-dimensional points
    Point **points = createFeaturesPoints(f1, f2, featureSize);
    if (cor >= 0.9) {
        float max = 0;
        //create the linear regression
        Line l = linearReg(f1, f2, featureSize);
        //check the greatest distance
        for (int i = 0; i < featureSize; ++i) {
            float distance = dev(*points[i], l);
            if (distance > max) {
                max = distance;
            }
        }
        //free points array
        deletePoint(points, featureSize);
        return max;
    }
    else {
        Circle c = findMinCircle(points, featureSize);
        float radius = c.radius;
        deletePoint(points, featureSize);
        return radius;
    }
}


vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> reports;
    int correlationSize = this->correlation->size();
    for (int i = 0 ; i < correlationSize ; ++i) {
        correlatedFeatures cur = this->correlation->at(i);
        int sizeOfPoints = ts.getFeatureValues(cur.feature1).size();
        Point **points = createFeaturesPoints(ts.getFeatureValues(cur.feature1),
                                              ts.getFeatureValues(cur.feature2), sizeOfPoints);
        if (cur.corrlation >= 0.9) {
            for (int j = 0; j < sizeOfPoints; ++j) {
                if (dev(*points[j], cur.lin_reg) > (cur.threshold * 1.1)) {
                    AnomalyReport report(cur.feature1 + "-" + cur.feature2, j + 1);
                    reports.push_back(report);
                }
            }
        }
        else {
            Circle c = findMinCircle(points,sizeOfPoints);
            Point center = c.center;
            for (int j = 0; j < sizeOfPoints; ++j) {
                if (distance(*points[j] , center) > (cur.threshold * 1.1)) {
                    AnomalyReport report(cur.feature1 + "-" + cur.feature2, j + 1);
                    reports.push_back(report);
                }
            }
        }
        deletePoint(points, sizeOfPoints);
    }
    return reports;
}

