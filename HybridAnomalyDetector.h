

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    virtual ~HybridAnomalyDetector();
    virtual float corThreshold();
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);
    virtual float getThreshold(vector<float> &feature1, vector<float> &feature2, int featureSize, float cor);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
