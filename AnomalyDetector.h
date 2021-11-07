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
    //constructor via file
    TimeSeries(const std::string& file);

    /*
    * function return a specified feature's values
    * param: string feature - the specified feature
    * return vector<float> - all of the feature's values
    */
    vector<float>getFeatureValues(string feature) const;

    /*
    * function returns a vector of all of the features
    * param: none
    * return vector<string> - all of the features
    */
    vector<string> getFeatures() const;

    /*
    * function return the data
    * param: none
    * return const vector<pair<string , vector<float>>>& - the data of the timeseries
    */
    vector<pair<string , vector<float>>> getData() const;

private:
    /*
     * function initial all of the features
     * param: const string& line - a new line with the features to add to the TimeSeries
     * return none
    */
    void addFeature(const std::string& line);

    /*
    * function adds all of the features values in a single line to the time series
    * param: const string& line - a new line with the values to add to the TimeSeries
    * return none
    */
    void updateData(const std::string& values);
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
    virtual ~TimeSeriesAnomalyDetector()= default;
};

#endif //ANOMALY_DETECTION_UTIL_CPP_ANOMALYDETECTOR_H
