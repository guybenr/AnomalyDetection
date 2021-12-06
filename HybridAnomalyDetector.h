

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
public:
    ~HybridAnomalyDetector() override;
    correlatedFeatures * getCorrelated(int current, vector<pair<string, vector<float>>> &data, int sizeData) override;
    vector<AnomalyReport> detect(const TimeSeries &ts) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
