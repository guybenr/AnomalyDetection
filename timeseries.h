
#include <vector>
#include <iostream>

#ifndef ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
#define ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class TimeSeries {
    vector <pair<string , vector<float>>> data{};
public:
    TimeSeries(const string& file);
    void addFeature(const string& line);
    void updateData(const string& values);

};


#endif //ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
