
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
    TimeSeries(const char* CSVfileName);

    /**
    * function return a specified feature's values
    * @param string feature - the specified feature
    * @return vector<float> - all of the feature's value
    */
    vector<float>getFeatureValues(string feature) const;

    /**
    * function returns a vector of all of the features
    * @param none
    * @return vector<string> - all of the features
    */
    vector<string> getFeatures () const;

    /**
    * function return the data
    * @param none
    * @return const vector<pair<string , vector<float>>>& - the data of the timeseries
    */
    vector<pair<string , vector<float>>> getData() const;

private:
    /**
     * function initial all of the features
     * @param const string& line - a new line with the features to add to the TimeSeries
     * @return none
    */
    void addFeature(const string& line);

    /**
    * function adds all of features values in a single line to the time series
    * @param const string& line - a new line with the values to add to the TimeSeries
    * @return none
    */
    void updateData(const string& values);
};


#endif //ANOMALY_DETECTION_UTIL_CPP_TIMESERIES_H
