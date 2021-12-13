//
// Created by adida on 13/12/2021.
//
#include "commands.h"
#include <filesystem>




UploadCommand::UploadCommand(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void UploadCommand::execute() {
    Command::dio->write("Please upload your local train CSV file.\n");
    string path = "train.csv";
    Command::dio->writeToFile(path);
    TimeSeries trainTs(path.c_str());
    Command::dio->write("Please upload your local train CSV file.\n");
    path = "test.csv";
    TimeSeries* testTs = new TimeSeries(path.c_str());
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
    Command::dio->write("complete detection anomaly.\n");
}


Display::Display(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void Display::execute() {
    vector<AnomalyReport> reports = *Command::info->reports;
    for (AnomalyReport report : reports) {
        Command::dio->write(report.timeStep);
        Command::dio->write("   ");
        Command::dio->write(report.description);
        Command::dio->write("\n");
    }
    Command::dio->write("Done.\n");

}

Analyze::Analyze(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void Analyze::execute() {


}



