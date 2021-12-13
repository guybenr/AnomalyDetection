
#include "HybridAnomalyDetector.h"


HybridAnomalyDetector::~HybridAnomalyDetector() = default;

correlatedFeatures* HybridAnomalyDetector::getCorrelated
            (int current ,vector<pair<string,vector<float>>>& data , int sizeData) {
    //the minimum value indicating correlation
    float max = 0.5;
    int maxFeature = -1;
    //how many values
    int sizeValues = data[current].second.size();
    //over all feature from current vector until the end and check which one is the most correlated
    for(int i = current + 1 ; i < sizeData ; ++i) {
        float cor = pearson(&data[current].second[0], &data[i].second[0], sizeValues);
        if (cor >= max) {
            max = cor;
            maxFeature = i;
        }
    }
    //if there is no correlative feature
    if (maxFeature == -1) {
        return nullptr;
    }
    //create the correlation according the struct
    pair<string, vector<float>>& feature1 = data[current];
    pair<string, vector<float>>& feature2 = data[maxFeature];
    if (max >= getCorThreshold()) {
        float threshold = getThreshold(feature1.second, feature2.second, sizeValues, max);
        Line linearReg = this->linearReg(feature1.second, feature2.second, sizeValues);
        correlatedFeatures *features = createCorrelatedFeatures
                (feature1.first, feature2.first, linearReg, Circle(), max, threshold);
        return features;
    }
    Point** points = createFeaturesPoints(feature1.second, feature2.second, sizeValues);
    Circle minCircle = findMinCircle(points, sizeValues);
    correlatedFeatures *features = createCorrelatedFeatures
            (feature1.first, feature2.first, Line(),
             minCircle, max, minCircle.radius);
    deletePoint(points, sizeValues);
    return features;
}


vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries &ts) {
    vector<AnomalyReport> reports;
    int correlationSize = this->correlation->size();
    for (int i = 0 ; i < correlationSize ; ++i) {
        correlatedFeatures cur = this->correlation->at(i);
        int sizeOfPoints = ts.getFeatureValues(cur.feature1).size();
        Point **points = createFeaturesPoints(ts.getFeatureValues(cur.feature1),
                                              ts.getFeatureValues(cur.feature2), sizeOfPoints);
        // if the corrlation >= 0.9 then its a Linear regression
        if (cur.corrlation >= getCorThreshold()) {
            for (int j = 0; j < sizeOfPoints; ++j) {
                if (dev(*points[j], cur.lin_reg) > (cur.threshold * 1.1)) {
                    AnomalyReport report(cur.feature1 + "-" + cur.feature2, j + 1);
                    reports.push_back(report);
                }
            }
        }
        // else its a circle
        else {
            Circle c = cur.circle;
            Point center = c.center;
            for (int j = 0; j < sizeOfPoints; ++j) {
                // searching for points outside of the circle
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

