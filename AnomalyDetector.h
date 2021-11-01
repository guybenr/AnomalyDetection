//
// Created by adida on 01/11/2021.
//

#ifndef ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H
#define ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class TimeSeries {
    std::vector <std::pair<std::string , std::vector<float>>> data{};
public:
    TimeSeries(const std::string& file);
    void addFeature(const std::string& line);
    void updateData(const std::string& values);
    vector<float>getFeatureValues(string feature);
    vector<string> getFeatures() const;
    vector<pair<string , vector<float>>> getData() const;
};
class AnomalyReport{
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep) :
            description(description),timeStep(timeStep){}
};
class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};

#endif //ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H
