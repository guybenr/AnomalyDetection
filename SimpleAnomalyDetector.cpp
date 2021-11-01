//
// Created by adida on 01/11/2021.
//
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {

};


correlatedFeatures SimpleAnomalyDetector::createStruct(string current , string second , Line l , float cor , float th) {
    struct correlatedFeatures *clp = new correlatedFeatures;
    clp->feature1 = current;
    clp->feature2 = second;
    clp->lin_reg = l;
    clp->threshold = th;
    return *clp;
}

Line linearReg(vector<float> feature1, vector<float> feature2, int featureSize) {
    Point **points = new Point*[featureSize];
    for (int i = 0; i < featureSize; ++i) {
        points[i] = new Point(feature1[i], feature2[i]);
    }
    Line linearReg = linear_reg(points, featureSize);
    for (int i = 0; i < featureSize; ++i) {
        delete points[i];
    }
    delete[] points;
    return linearReg;
}

float SimpleAnomalyDetector::threshold(vector<float> feature1, vector<float> feature2, int featureSize) {
    Point **points = new Point*[featureSize];
    for (int i = 0; i < featureSize; ++i) {
        points[i] = new Point(feature1[i], feature2[i]);
    }
    float max = 0;
    Line l = linearReg(feature1 , feature2 , featureSize);
    for (int i = 0 ; i < featureSize ; ++i) {
        if (dev(*points[i] , l) > 0) {
            max = dev(*points[i] , l);
        }
    }
    return max;
}

correlatedFeatures SimpleAnomalyDetector::getCorrelated(int current , vector<pair<string,vector<float>>>& data , int sizeData) {
    float max = 0;
    int maxFeature;
    int sizeValues = data[current].second.size();
    for(int i = current + 1 ; i < sizeData ; ++i) {
        float *currentValues = &data[current].second[0];
        float *nextValues = &data[i].second[0];
        float cor = pearson(currentValues, nextValues, sizeValues);
        if (cor > max) {
            max = cor;
            maxFeature = i;
        }
    }

}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    vector<pair<string,vector<float>>> data = ts.getData();
    int sizeData = data.size();
    for(int i = 0 ; i < sizeData ; ++i) {
        getCorrelated(i , data , sizeData);
    }

}
