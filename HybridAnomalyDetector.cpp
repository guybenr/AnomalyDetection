
#include "HybridAnomalyDetector.h"


HybridAnomalyDetector::~HybridAnomalyDetector() {
}

float HybridAnomalyDetector::corThreshold() {
    return 0.5;
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
                cur.threshold = c.radius;
                if (distance(*points[j] , center) > (c.radius * 1.1)) {
                    AnomalyReport report(cur.feature1 + "-" + cur.feature2, j + 1);
                    reports.push_back(report);
                }
            }
        }
        deletePoint(points, sizeOfPoints);
    }
    return reports;
}

