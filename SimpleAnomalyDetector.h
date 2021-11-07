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
    vector <correlatedFeatures> *correlation{};
public:
    //constructor
    SimpleAnomalyDetector();
    //destructor
    virtual ~SimpleAnomalyDetector();

    /*
     * function adds all of the timeseries's correlatedFeatures to the correlation member
     * param: const TimeSeries& ts - the time to learn
     * return none
     */
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    vector<correlatedFeatures> getNormalModel();
    correlatedFeatures* getCorrelatedTo(string feature);

private:
    correlatedFeatures* getCorrelated(int current , vector<pair<string,vector<float>>>& data , int sizeData);
    float getThreshold(vector<float>& feature1, vector<float>& feature2, int featureSize);
    Line linearReg(vector<float>& feature1, vector<float>& feature2, int featureSize);
    Point** createFeaturesPoints(vector<float>& feature1, vector<float>& feature2, int featureSize);
    };

#endif //ANOMALY_DETECTION_UTIL_CPP_SIMPLEANOMALYDETECTOR_H
