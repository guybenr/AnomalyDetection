//
// Created by adida on 31/10/2021.
//
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

#include "timeseries.h"
timeseries::timeseries(const string file) {
    fstream f(file);
    string line;
    getline(f, line);
    addFeature(line);
    while (true) {
        string values;
        getline(f, values);
        if (values.size() == 0) {
            break;
        }
        updateData(values);
    }
}
void timeseries::addFeature(string line) {
    int start = 0, end = line.find(',');
    while (true) {
        string feature = line.substr(start, end - start);
        start = end + 1;
        end = line.find(',', start);
        pair<string,vector<float>> p;
        p.first = feature;
        this->data.push_back(p);
        if (end == -1) {
            feature = line.substr(start, line.find('\n', start));
            pair<string,vector<float>> p;
            p.first = feature;
            this->data.push_back(p);
            break;
        }
    }
}

void timeseries::updateData(std::string values) {
    vector<pair<string, vector<float>>>::iterator it = this->data.begin();
    int start = 0, end = values.find(',');
    while (true) {
        float value = stof(values.substr(start, end - start));
        it->second.push_back(value);
        ++it;
        start = end + 1;
        end = values.find(',', start);
        if (end == -1) {
            float value = stof(values.substr(start, '\n'));
            it->second.push_back(value);
            break;
        }
    }
}

