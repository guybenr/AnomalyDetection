//
// Created by adida on 01/11/2021.
//

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


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

private:
    correlatedFeatures* getCorrelated(int current , vector<pair<string,vector<float>>>& data , int sizeData);
    float getThreshold(vector<float>& feature1, vector<float>& feature2, int featureSize);
    Line linearReg(vector<float>& feature1, vector<float>& feature2, int featureSize);
    Point** createFeaturesPoints(vector<float>& feature1, vector<float>& feature2, int featureSize);
    };


#endif /* SIMPLEANOMALYDETECTOR_H_ */
