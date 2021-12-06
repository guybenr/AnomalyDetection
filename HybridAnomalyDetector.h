

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    virtual ~HybridAnomalyDetector();
    virtual float corThreshold();
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
