//
// Created by adida on 01/11/2021
//
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->correlation = new vector<correlatedFeatures>;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    delete this->correlation;
}

correlatedFeatures* SimpleAnomalyDetector:: createCorrelatedFeatures
            (string current , string second , Line l, Circle c, float cor , float th) {
    correlatedFeatures *clp = new correlatedFeatures;
    clp->feature1 = current;
    clp->feature2 = second;
    clp->corrlation = cor;
    clp->lin_reg = l;
    clp->threshold = th;
    clp->circle = c;
    return clp;
}

void SimpleAnomalyDetector::deletePoint(Point **points , int size) {
    //over all point in points and free it
    for (int i = 0; i < size; ++i) {
        delete points[i];
    }
    //free the points array
    delete[] points;
}

Point** SimpleAnomalyDetector::createFeaturesPoints(vector<float> f1, vector<float> f2, int featureSize) {
    Point **points = new Point*[featureSize];
    //create the points
    for (int i = 0; i < featureSize; ++i) {
        points[i] = new Point(f1[i], f2[i]);
    }
    return points;
}

Line SimpleAnomalyDetector::linearReg(vector<float>& f1, vector<float>& f2, int featureSize) {
    //create the two-dimensional points
    Point **points = createFeaturesPoints(f1, f2, featureSize);
    //create the linear regression
    Line linearReg = linear_reg(points, featureSize);
    //free points array
    deletePoint(points,featureSize);
    return linearReg;
}

float SimpleAnomalyDetector::getThreshold(vector<float>& f1, vector<float>& f2, int featureSize, float cor) {
    //create the two-dimensional points
    Point **points = createFeaturesPoints(f1, f2, featureSize);
    float max = 0;
    //create the linear regression
    Line l = linearReg(f1 , f2 , featureSize);
    //check the greatest distance
    for (int i = 0 ; i < featureSize ; ++i) {
        float distance = dev(*points[i] , l);
        if (distance > max) {
            max = distance;
        }
    }
    //free points array
    deletePoint(points,featureSize);
    return max;
}

correlatedFeatures* SimpleAnomalyDetector::getCorrelated(int current ,vector<pair<string,vector<float>>>& data , int sizeData) {
    //the minimum value indicating correlation
    float max = this->corThreshold;
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
    float threshold = getThreshold(feature1.second, feature2.second, sizeValues, max);
    Line linearReg = this->linearReg(feature1.second, feature2.second, sizeValues);
    correlatedFeatures* features = createCorrelatedFeatures
            (feature1.first, feature2.first, linearReg, Circle(), max, threshold);
    return features;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    vector<pair<string,vector<float>>> data = ts.getData();
    int sizeData = data.size();
    for(int i = 0 ; i < sizeData ; ++i) {
        correlatedFeatures* cf = getCorrelated(i , data , sizeData);
        if(cf == nullptr)
            continue;
        this->correlation->push_back(*cf);
        delete cf;
    }
}

float SimpleAnomalyDetector :: getCorThreshold() {
    return this->corThreshold;
}

void SimpleAnomalyDetector :: setCorThreshold(float corThreshold) {
    this->corThreshold = corThreshold;
}


vector<AnomalyReport> SimpleAnomalyDetector:: detect(const TimeSeries& ts) {
    vector<AnomalyReport> reports;
    int correlationSize = this->correlation->size();
    for (int i = 0 ; i < correlationSize ; ++i) {
        correlatedFeatures cur = this->correlation->at(i);
        int sizeOfPoints = ts.getFeatureValues(cur.feature1).size();
        Point **points = createFeaturesPoints(ts.getFeatureValues(cur.feature1),
                                              ts.getFeatureValues(cur.feature2), sizeOfPoints);
        for (int j = 0; j < sizeOfPoints; ++j) {
            if (dev(*points[j], cur.lin_reg) > (cur.threshold * 1.1)) {
                AnomalyReport report(cur.feature1 + "-" + cur.feature2, j + 1);
                reports.push_back(report);
            }
        }
        deletePoint(points, sizeOfPoints);
    }
    return reports;
}

vector<correlatedFeatures> SimpleAnomalyDetector:: getNormalModel() {
    return *this->correlation;
}





