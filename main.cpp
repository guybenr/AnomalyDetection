#include <iostream>
#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"


using namespace std;

// this is a simple test to put you on the right track
int main(){
    auto *t2 = new TimeSeries("C:\\Users\\adida\\CLionProjects\\TimeSeriesData\\airtravel.csv");
    auto *t = new TimeSeries("C:\\Users\\adida\\CLionProjects\\TimeSeriesData\\addresses.csv");
    TimeSeriesAnomalyDetector *s = new SimpleAnomalyDetector();
    s->learnNormal(*t);
    s->detect(*t2);
    return 0;
}

