//
// Created by adida on 31/10/2021.
//
#include <vector>
#include <fstream>
#include <string>

using namespace std;

#include "timeseries.h"
TimeSeries::TimeSeries(const string& file) {
    fstream f(file);
    string line;
    getline(f, line);
    addFeature(line);
    while (true) {
        string values;
        getline(f, values);
        if (values.empty()) {
            break;
        }
        updateData(values);
    }
}
void TimeSeries::addFeature(const string& line) {
    int start = 0, end = line.find(',');
    do {
        string feature = line.substr(start, end - start);
        start = end + 1;
        end = (end == -1) ? line.find('\n', start) : line.find(',', start);
        pair<string, vector<float>> p;
        p.first = feature;
        this->data.push_back(p);
    } while (start != 0);
}

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

