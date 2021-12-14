//
// Created by adida on 13/12/2021.
//
#include "commands.h"
#include <algorithm>


UploadCommand::UploadCommand(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

string UploadCommand::getDes() {
    string s = "1.upload a time series csv file\n";
    return s;
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

string AlgorithmSettings::getDes() {
    string s = "2.algorithm settings\n";
    return s;
}

void AlgorithmSettings::execute() {
    Command::dio->write("The current correlation threshold is ");
    Command::dio->write(this->info->detector->getCorThreshold());
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

string DetectAnomalies::getDes() {
    string s = "3.detect anomalies\n";
    return s;
}

void DetectAnomalies::execute() {
    vector<AnomalyReport> reports = Command::info->detector->detect(*Command::info->ts);
    Command::info->reports = new vector<AnomalyReport>(reports);
    Command::dio->write("complete detection anomaly.\n");
}


Display::Display(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

string Display::getDes() {
    string s = "4.display results\n";
    return s;
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

string Analyze::getDes() {
    string s = "5.upload anomalies and analyze results\n";
    return s;
}

vector<pair<int, int>> Analyze::updateAnomalies(int &P, int &N, std::ifstream &anomaliesTxt) {
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
            anomalies.push_back(pair<int, int>(x, y));
            getline(anomaliesTxt, line);
        }
        N = Command::info->ts->getData()[0].second.size() - N;
    }
    return anomalies;
}


vector<pair<int, int>> Analyze::NoAnomalies(vector<pair<int, int>> anomalies) {
    string line;
    vector<pair<int, int>> NoAnomalies;
    int start = 0;
    for (pair<int, int> p: anomalies) {
        if (start < p.first - 1) {
            NoAnomalies.push_back(pair<int, int>(start, p.first - 1));
        }
        start = p.second + 1;
    }
    NoAnomalies.push_back(pair<int,int>(start , Command::info->ts->getData()[0].second.size()));
    return NoAnomalies;
}


void  Analyze::updateFPandTP(int &FP, int &TP, vector<pair<int, int>> *anomalies, vector<pair<int, int>> *unionAnomalies) {
    for (pair<int, int> pCheck: *unionAnomalies) {
        bool flagTP = false;
        for (pair<int, int> pReal: *anomalies) {
            if (pCheck.second >= pReal.first || pCheck.first >= pReal.second) {
                TP += 1;
                flagTP = true;
                break;
            }
        }
        if (!flagTP)
            FP += 1;
    }
}


void  Analyze::updateFNorTN(int &P, vector<pair<int, int>> *vector1, vector<pair<int, int>> *vector2) {
    for (pair<int, int> pCheck: *vector1) {
        bool flagTN = false;
        for (pair<int, int> pReal: *vector2) {
            if (pCheck.second >= pReal.first || pCheck.first >= pReal.second) {
                flagTN = true;
                break;
            }
        }
        if (!flagTN)
            P += 1;
    }
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
    vector<pair<int, int>> NoAnomalies = this->NoAnomalies(anomalies);
    vector<pair<int, int>> unionAnomalies = this->getUnionReports();
    int FP = 0;
    int TP = 0;
    int FN = 0;
    int TN = 0;
    updateFPandTP(FP, TP, &anomalies, &unionAnomalies);
    updateFNorTN(TN, &NoAnomalies, &unionAnomalies);
    updateFNorTN(FN, &anomalies, &unionAnomalies);
    float ftRate = TP/P;
    float fRate = FP/N;
    Command::dio->write("True Positive Rate:");
    Command::dio->write(ftRate);
    Command::dio->write("\n");
    Command::dio->write("False Positive Rate:");
    Command::dio->write(fRate);
    Command::dio->write("\n");
}

int comp(AnomalyReport a,AnomalyReport b) {
    if ((long) a.description.compare(b.description) == 0) {
        return a.timeStep - b.timeStep;
    }
    return (long) a.description.compare(b.description);
}

vector<pair<int, int>> Analyze:: getUnionReports() {
    vector<AnomalyReport> *reports = Command::info->reports;
    vector<pair<int, int>> unionReports;
    sort(reports->begin(), reports->end(), comp);
    if (reports->empty())
        return unionReports;
    AnomalyReport *lastReport = &(*reports)[0];
    int size = reports->size();
    for (int i = 0; i < size; ++i) {
        AnomalyReport report = (*reports)[i];
        if (!lastReport->description.compare(report.description)) {
            unionReports.push_back(pair<int, int>(lastReport->timeStep, (*reports)[i - 1].timeStep));
            lastReport = &(*reports)[i];
        } else if (report.timeStep - lastReport->timeStep != 1) {
            unionReports.push_back(pair<int, int>(lastReport->timeStep, (*reports)[i - 1].timeStep));
            lastReport = &(*reports)[i];
        }
    }
    return unionReports;
}


Exit::Exit(DefaultIO *dio, infoCommand *info) : Command(dio) {
    Command::info = info;
}

string Exit::getDes() {
    string s = "6.exit\n";
    return s;
}

void Exit::execute() {
    return;
}