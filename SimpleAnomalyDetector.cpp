//
// Created by adida on 01/11/2021.
//
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    this->correlation = new vector<correlatedFeatures>;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
    delete this->correlation;
}

correlatedFeatures* createCorrelatedFeatures(string current , string second , Line l , float cor , float th) {
    correlatedFeatures *clp = new correlatedFeatures;
    clp->feature1 = current;
    clp->feature2 = second;
    clp->corrlation = cor;
    clp->lin_reg = l;
    clp->threshold = th;
    return clp;
}

Point** SimpleAnomalyDetector::createFeaturesPoints(vector<float>& feature1, vector<float>& feature2, int featureSize) {
    Point **points = new Point*[featureSize];
    for (int i = 0; i < featureSize; ++i) {
        points[i] = new Point(feature1[i], feature2[i]);
    }
    return points;
}

Line SimpleAnomalyDetector::linearReg(vector<float>& feature1, vector<float>& feature2, int featureSize) {
    Point **points = createFeaturesPoints(feature1, feature2, featureSize);
    Line linearReg = linear_reg(points, featureSize);
    for (int i = 0; i < featureSize; ++i) {
        delete points[i];
    }
    delete[] points;
    return linearReg;
}

float SimpleAnomalyDetector::getThreshold(vector<float>& feature1, vector<float>& feature2, int featureSize) {
    Point **points = createFeaturesPoints(feature1, feature2, featureSize);
    float max = 0;
    Line l = linearReg(feature1 , feature2 , featureSize);
    for (int i = 0 ; i < featureSize ; ++i) {
        float distance = dev(*points[i] , l);
        if (distance > max) {
            max = distance;
            delete points[i];
        }
    }
    delete[] points;
    return max;
}

correlatedFeatures* SimpleAnomalyDetector::getCorrelated(int current ,vector<pair<string,vector<float>>>& data , int sizeData) {
    float max = 0.9;
    int maxFeature = -1;
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
    if (maxFeature == -1) {
        return nullptr;
    }
    pair<string, vector<float>>& feature1 = data[current];
    pair<string, vector<float>>& feature2 = data[maxFeature];
    float threshold = getThreshold(feature1.second, feature2.second, sizeValues);
    Line linearReg = this->linearReg(feature1.second, feature2.second, sizeValues);
    correlatedFeatures* features = createCorrelatedFeatures(feature1.first, feature2.first, linearReg, max, threshold);
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
    }
}


vector<AnomalyReport> SimpleAnomalyDetector:: detect(const TimeSeries& ts) {
    vector<AnomalyReport> *reports = new vector<AnomalyReport>;
    int correlationSize = this->correlation->size();
    for (int i = 0 ; i < correlationSize ; ++i) {
        correlatedFeatures cur = this->correlation->at(i);
        string featureOne = cur.feature1;
        string featureTwo = cur.feature2;
        vector<float> valuesOne = ts.getFeatureValues(featureOne);
        vector<float> valuesTwo = ts.getFeatureValues(featureTwo);
        int sizeOfPoints = valuesOne.size();
        Point **points = createFeaturesPoints(valuesOne, valuesTwo, sizeOfPoints);
        for (int j = 0; j < sizeOfPoints; ++j) {
            if (dev(*points[j], cur.lin_reg) > (cur.threshold * 1.1)) {
                AnomalyReport *report = new AnomalyReport(featureOne + "-" + featureTwo, j + 1);
                reports->push_back(*report);
            }
        }
    }
    return *reports;
}

vector<correlatedFeatures> SimpleAnomalyDetector:: getNormalModel() {
    return *this->correlation;
}





