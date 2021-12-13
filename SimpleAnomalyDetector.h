//
// Created by adida on 01/11/2021
//

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include "minCircle.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>


struct correlatedFeatures {
    string feature1, feature2;
    float corrlation;
    Line lin_reg;
    float threshold;
    Circle circle;
};

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {
protected:
    vector<correlatedFeatures> *correlation{};
private:
    float corThreshold = 0.9;
public:
    //constructor
    SimpleAnomalyDetector();

    //return corThreshold
    float getCorThreshold();

    //set corThreshold
    void setCorThreshold(float corThreshold);

    //destructor
    virtual ~SimpleAnomalyDetector();

    /**
     * function adds all of the time series's correlatedFeatures to the correlation member
     * @param const TimeSeries& ts - the time to learn
     * @return none
     */
    virtual void learnNormal(const TimeSeries &ts);

    /**
     * function check exception for new values (time series ts) according to liner regression
     * @param const TimeSeries& ts - the new values
     * @return vector of exception reports
     */
    virtual vector<AnomalyReport> detect(const TimeSeries &ts);

    /**
     * function return the vector of correlation features
     * @return vector of correlation features
     */
    vector<correlatedFeatures> getNormalModel();


    /**
     * function check which feature is the most correlated to specific feature
     * @param current the index of specific feature in the vector of data
     * @param data the vector of all feature with their values
     * @param sizeData the size of feature vector
     * @return pointer to new correlated feature of the specific feature with another feature
     */
    virtual correlatedFeatures *getCorrelated(int current, vector<pair<string, vector<float>>> &data, int sizeData);

    /**
     * function free memory of array of points
     * @param points array of points
     */
    static void deletePoint(Point **points, int size);


/**
 * function check what is the greatest distance between the points of
 * two correlative feature and their regression line
 * @param feature1 the values of first feature
 * @param feature2 the values of second feature
 * @param featureSize the size of values vector
 * @return the greatest distance
 */
    virtual float getThreshold(vector<float> &feature1, vector<float> &feature2, int featureSize, float cor);

/**
 * function create linear regression for two features
 * @param feature1 the values of first feature
 * @param feature2 the values of second feature
 * @param featureSize the size of values vector
 * @return the linear regression
 */
    Line linearReg(vector<float> &feature1, vector<float> &feature2, int featureSize);

/**
 * function create new struct correlation
 * @param current the first feature
 * @param second the feature that correlative the first
 * @param l the line regression
 * @param cor the correlation number
 * @param th the greatest distance to regression line
 * @return pointer to new correlated feature of the specific feature with another feature
 */
    static correlatedFeatures *createCorrelatedFeatures(string current, string second, Line l, Circle c, float cor, float th);

/**
 * function create two-dimensional points from two feature values
 * @param feature1 the values of first feature
 * @param feature2 the values of second feature
 * @param featureSize the size of values vector
 * @return array of points
 */
    static Point **createFeaturesPoints(vector<float> feature1, vector<float> feature2, int featureSize);

};


#endif /* SIMPLEANOMALYDETECTOR_H_ */
