
#include <vector>
#include <iostream>

#ifndef ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
#define ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H


class timeseries {
    std::vector <std::pair<std::string , std::vector<float>>> data{};
public:
    timeseries(std::string file);
    void addFeature(std::string line);
    void updateData(std::string values);

};


#endif //ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
