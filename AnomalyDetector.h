//
// Created by adida on 01/11/2021
//

#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include <string>
#include <vector>
#include "timeseries.h"
#include "math.h"//
//

using namespace std;

class AnomalyReport{
public:
    string description;
    long timeStep;
    AnomalyReport(string description, long timeStep):description(description),timeStep(timeStep){}
};
class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector()= default;
};

#endif /* ANOMALYDETECTOR_H_ */
