#include <iostream>
#include "anomaly_detection_util.h"
#include "timeseries.h"


using namespace std;

// this is a simple test to put you on the right track
int main(){
    timeseries *t = new timeseries("C:\\Users\\adida\\CLionProjects\\TimeSeriesData\\addresses.csv");
    int d = -1;
    int p = (d == -1) ? 0 : 5;
    return 0;
}

