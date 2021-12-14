//
// Created by adida on 01/11/2021
//

#ifndef ANOMALY_DETECTION_UTIL_CPP_ANOMALYREPORT_H
#define ANOMALY_DETECTION_UTIL_CPP_ANOMALYREPORT_H

#include <vector>
#include <fstream>
#include <string>

using namespace std;

class AnomalyReport{
public:
    string description;
    long timeStep;
    AnomalyReport(string description, long timeStep) :
            description(description),timeStep(timeStep){}
};


#endif //ANOMALY_DETECTION_UTIL_CPP_ANOMALYREPORT_H
