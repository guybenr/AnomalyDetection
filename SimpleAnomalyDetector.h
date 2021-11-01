//
// Created by adida on 01/11/2021.
//

#ifndef ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H
#define ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H

#include <vector>
#include <fstream>
#include <string>
#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
using namespace std;

struct correlatedFeatures{
    string feature1,feature2; // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};
class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector <correlatedFeatures> correlation{};
public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel();
    correlatedFeatures getCorrelated(int current , vector<pair<string,vector<float>>>& data , int sizeData);
    correlatedFeatures createStruct(string current , string second , Line l , float cor , float th)
};

#endif //ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H
