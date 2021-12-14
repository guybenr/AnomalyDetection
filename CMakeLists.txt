cmake_minimum_required(VERSION 3.20)
project(AnomalyDetection)

set(CMAKE_CXX_STANDARD 11)

add_executable(AnomalyDetection main.cpp anomaly_detection_util.cpp anomaly_detection_util.h timeseries.cpp timeseries.h main.cpp main.cpp AnomalyDetector.h SimpleAnomalyDetector.h AnomalyReport.cpp AnomalyReport.h SimpleAnomalyDetector.cpp minCircle.cpp minCircle.h HybridAnomalyDetector.cpp HybridAnomalyDetector.h commands.cpp CLI.cpp)
