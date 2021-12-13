//
// Created by adida on 13/12/2021.
//
#include "commands.h"


UploadCommand::UploadCommand(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void UploadCommand::execute() {
    Command::dio->write("Please upload your local train CSV file.\n");
    const char* path = Command::dio->read().c_str();
    TimeSeries trainTs(path);
    Command::dio->write("Please upload your local train CSV file.\n");
    path = Command::dio->read().c_str();
    TimeSeries* testTs = new TimeSeries(path);
    SimpleAnomalyDetector* simpleAnomalyDetector = new SimpleAnomalyDetector();
    Command::info->detector = simpleAnomalyDetector;
    simpleAnomalyDetector->learnNormal(trainTs);
    Command::info->ts = testTs;
}

AlgorithmSettings::AlgorithmSettings(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void AlgorithmSettings::execute() {

}


DetectAnomalies::DetectAnomalies(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void DetectAnomalies::execute() {
    vector<AnomalyReport> reports = Command::info->detector->detect(*Command::info->ts);
    Command::info->reports = new vector<AnomalyReport>(reports);

}



