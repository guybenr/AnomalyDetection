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
    TimeSeries *testTs = new TimeSeries(path.c_str());
    SimpleAnomalyDetector *simpleAnomalyDetector = new SimpleAnomalyDetector();
    Command::info->detector = simpleAnomalyDetector;
    simpleAnomalyDetector->learnNormal(trainTs);
    Command::info->ts = testTs;
    Command::dio->write("Upload complete.\n");
}

AlgorithmSettings::AlgorithmSettings(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

void AlgorithmSettings::execute() {
    Command::dio->write("The current correlation threshold is ");
    Command::dio->write( this->info->detector->getCorThreshold());
    Command::dio->write("\n");
    string inputThrString = Command::dio->read();
    float inputThr = stof(inputThrString);
    if (inputThr > 0 && inputThr < 1) {
        this->info->detector->setCorThreshold(inputThr);
    }
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
    for (AnomalyReport report: reports) {
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



vector<pair<int, int>> Analyze:: updateAnomalies(int &P, int &N, std::ifstream anomaliesTxt) {
    string line;
    vector<pair<int, int>> anomalies;
    if (anomaliesTxt.is_open()) {
        getline(anomaliesTxt, line);
        int end = line.find(","); //index of comma
        while (line != "done") {
            P++; // another row
            int x = stoi(line.substr(0, end)); // start time
            int y = stoi(line.substr(end + 1, line.size())); // end time
            N += (y - x + 1);
            anomalies.push_back(pair(x, y));
            getline(anomalies, line);
        }
        N = Command::info->ts->getData()[0].second.size() - N;
    }
    return anomalies;
}

void Analyze::execute() {
    Command::dio->write("Please upload your local anomalies CSV file.\n");
    string path = "anomalies.txt";
    Command::dio->writeToFile(path);
    std::ifstream anomaliesTxt;
    anomaliesTxt.open(path);
    //num of row=P , N = no detection
    int P = 0;
    int N = 0;
    vector<pair<int, int>> anomalies = this->updateAnomalies(P, N, anomaliesTxt);

    }


    vector<AnomalyReport> reports = *Command::info->reports;
    for (AnomalyReport report: reports) {
        Command::dio->write(report.timeStep);
        Command::dio->write("   ");
        Command::dio->write(report.description);
        Command::dio->write("\n");
    }

}



