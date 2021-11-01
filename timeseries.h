
#include <vector>
#include <iostream>

#ifndef ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
#define ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H


class TimeSeries {
    std::vector <std::pair<std::string , std::vector<float>>> data{};
public:
    TimeSeries(const std::string& file);
    void addFeature(const std::string& line);
    void updateData(const std::string& values);

};


#endif //ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
