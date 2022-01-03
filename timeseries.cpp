//
// Created by adida on 31/10/2021
//

#include "timeseries.h"

//constructor
TimeSeries::TimeSeries(const char* CSVfileName) {
    fstream f(CSVfileName);
    string line;
    getline(f, line);
    addFeature(line);// adding features
    while (true) {
        string values;
        getline(f, values);
        getline(f, values);
        if (values.empty()) {
            break;
        }
        updateData(values); // adding each line of values
    }
    f.close();
}

/**
 * function initial all of the features
 * @param const string& line - a new line with the features to add to the TimeSeries
 * @return none
*/
void TimeSeries::addFeature(const string& line) {
    int start = 0, end = line.find(',');
    do {
        string feature = line.substr(start, end - start);
        start = end + 1;
        end = (end == -1) ? line.find('\n', start) : line.find(',', start); // reads each feature
        pair<string, vector<float>> p;
        p.first = feature;
        this->data.push_back(p);
    } while (start != 0);
}

/**
* function adds all of the features values in a single line to the time series
* @param const string& line - a new line with the values to add to the TimeSeries
* @return none
*/
void TimeSeries::updateData(const std::string& values) {
    auto it = this->data.begin();
    int start = 0, end = values.find(',');
    do {
        float value = stof(values.substr(start, end - start));
        start = end + 1;
        end = (end == -1) ? values.find('\n' , start) : values.find(',', start);
        it->second.push_back(value);
        ++it;
    } while (start != 0);
}

/**
* function returns a vector of all of the features
* @param none
* @return vector<string> - all of the features
*/
vector<string> TimeSeries::getFeatures() const {
    vector<string> features;
    for(auto it = this->data.begin(); it < this->data.end(); it++) {
        features.push_back(it->first);
    }
    return features;
}

/**
* function return a specified feature's values
* @param string feature - the specified feature
* @return vector<float> - all of the feature's values
*/
vector<float> TimeSeries::getFeatureValues (string feature) const {
    for(auto it = this->data.begin(); it < this->data.end(); it++) {
        if(it->first == feature) {
            return it->second;
        }
    }
}

/**
 * function return the data
 * @param none
 * @return const vector<pair<string , vector<float>>>& - the data of the timeseries
 */
vector<pair<string , vector<float>>> TimeSeries :: getData() const {
    return this->data;
}