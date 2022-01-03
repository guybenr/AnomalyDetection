#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <algorithm>
#include <sys/socket.h>

using namespace std;

class DefaultIO{
public:
    virtual string read()=0;
    virtual void write(string text)=0;
    virtual void write(float f)=0;
    virtual void read(float* f)=0;
    void writeToFile(string path) {
        std::ofstream file;
        file.open(path);
        string row = this->read();
        while (row != "done\n") {
            file << row << "\n";
            row = this->read();
        }
        file.close();
    }
    void readFromFile(string path) {
        std::ifstream file;
        file.open(path);
        string row;
        getline(file, row);
        while (row != "done\n") {
            this->write(row);
            getline(file, row);
        }
        file.close();
    }
    virtual ~DefaultIO(){}

    // you may add additional methods here
};

class SocketIO: public DefaultIO{
private:
    int socketID;
public:
    SocketIO(int socketID) : socketID(socketID) {}
    virtual string read() override {
        char buf;
        string line = "";
        while(buf != '\n') {
            long bytes = recv(this->socketID, &buf, 1, 0);
            line += buf;
        }
        return line;
    }
    virtual void write(string text) override {
        send(this->socketID,text.c_str(),text.size(),0);
    }
    virtual void read(float *f) override {
        char buf = '1';
        string input = "";
        while (buf != '\n') {
            int bytes = recv(this->socketID, &buf, 1, 0);
            input += buf;
        }
        *f = stof(input.substr(0, input.size() - 1));
    }
    virtual void write(float f) override {
        string fStr = to_string(f);
        fStr.erase(fStr.find_last_not_of('0') + 1, std::string::npos);
        if (fStr[fStr.size() - 1] == '.')
            fStr = fStr.substr(0, fStr.size() - 2);
        send(this->socketID, fStr.c_str(), fStr.size(), 0);
    }
};

// you may add here helper classes

struct infoCommand {
    SimpleAnomalyDetector *detector = nullptr;
    TimeSeries *ts = nullptr;
    vector<AnomalyReport>* reports = nullptr;
};

// you may edit this class
class Command{
protected:
    DefaultIO* dio;
    infoCommand* info;

public:
    Command(DefaultIO* dio):dio(dio){}
    virtual void execute()=0;
    virtual string getDes()=0;
    virtual ~Command() {
        delete info->reports;
        delete info->ts;
        delete info->detector;
        delete info;
    }
};

// implement here your command classes

class UploadCommand:public Command {
public:
    UploadCommand(DefaultIO *dio, infoCommand *info): Command(dio){
        Command::info = info;
    }
    virtual string getDes() {
        string s = "1.upload a time series csv file\n";
        return s;
    }
    virtual void execute() {
        Command::dio->write("Please upload your local train CSV file.\n");
        string path = "train.csv";
        Command::dio->writeToFile(path);
        TimeSeries trainTs(path.c_str());
        Command::dio->write("Upload complete.\n");
        Command::dio->write("Please upload your local test CSV file.\n");
        path = "test.csv";
        Command::dio->writeToFile(path);
        TimeSeries *testTs = new TimeSeries(path.c_str());
        SimpleAnomalyDetector *simpleAnomalyDetector = new HybridAnomalyDetector();
        Command::info->detector = simpleAnomalyDetector;
        simpleAnomalyDetector->learnNormal(trainTs);
        Command::info->ts = testTs;
        Command::dio->write("Upload complete.\n");
    }
};

class AlgorithmSettings:public Command {
public:
    AlgorithmSettings(DefaultIO *dio, infoCommand *info): Command(dio) {
        Command::info = info;
    }
    virtual string getDes() {
        string s = "2.algorithm settings\n";
        return s;
    }
    virtual void execute() {
        Command::dio->write("The current correlation threshold is ");
        Command::dio->write(this->info->detector->getCorThreshold());
        Command::dio->write("\n");
        Command::dio->write("Type a new threshold\n");
        string inputThrString = Command::dio->read();
        float inputThr = stof(inputThrString);
        while (true) {
            if (inputThr > 0 && inputThr < 1) {
                this->info->detector->setCorThreshold(inputThr);
                break;
            }
            else {
                Command::dio->write("please choose a value between 0 and 1.\n");
                Command::dio->write("The current correlation threshold is ");
                Command::dio->write(this->info->detector->getCorThreshold());
                Command::dio->write("\n");
                inputThrString = Command::dio->read();
                inputThr = stof(inputThrString);
            }
        }
    }
};


class DetectAnomalies:public Command {
public:
    DetectAnomalies(DefaultIO *dio, infoCommand *info): Command(dio) {
        Command::info = info;
    }
    virtual string getDes() {
        string s = "3.detect anomalies\n";
        return s;
    }
    virtual void execute() {
        vector<AnomalyReport> reports = Command::info->detector->detect(*Command::info->ts);
        Command::info->reports = new vector<AnomalyReport>(reports);
        Command::dio->write("anomaly detection complete.\n");
    }
};


class Display:public Command {
public:
    Display(DefaultIO *dio, infoCommand *info): Command(dio) {
        Command::info = info;
    }
    virtual string getDes() {
        string s = "4.display results\n";
        return s;
    }
    virtual void execute() {
        vector<AnomalyReport> reports = *Command::info->reports;
        for (AnomalyReport report: reports) {
            Command::dio->write(report.timeStep);
            Command::dio->write("\t");
            Command::dio->write(report.description);
            Command::dio->write("\n");
        }
        Command::dio->write("Done.\n");
    }
};

class Analyze:public Command {
public:
    Analyze(DefaultIO *dio, infoCommand *info): Command(dio) {
        Command::info = info;
    }
    virtual string getDes() {
        string s = "5.upload anomalies and analyze results\n";
        return s;
    }
    virtual void execute() {
        Command::dio->write("Please upload your local anomalies file.\n");
        string path = "anomalies.txt";
        Command::dio->writeToFile(path);
        Command::dio->write("Upload complete.\n");
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
        float tRate = (float)TP/(float)P;
        float fRate = (float)FP/(float)N;
        tRate = roundOff((float)((int)(tRate*1000))/(float)1000 , 3);
        fRate =roundOff((float)((int)(fRate*1000))/(float)1000 , 3);
        string s;
        Command::dio->write("True Positive Rate: ");
        Command::dio->write(tRate);
        Command::dio->write("\nFalse Positive Rate: ");
        Command::dio->write(fRate);
        Command::dio->write("\n");
    }

private:
    float roundOff(float value, unsigned char prec) {
        float pow_10 = pow(10.0f, (float)prec);
        return round(value * pow_10) / pow_10;
    }
    vector<pair<int, int>> updateAnomalies(int &P, int &N, std::ifstream &anomaliesTxt) {
        string line;
        vector<pair<int, int>> anomalies;
        if (anomaliesTxt.is_open()) {
            getline(anomaliesTxt, line);
            int end; //index of comma
            while (line.compare("") != 0) {
                end = line.find(",");
                P++; // another row
                int x = stoi(line.substr(0, end)); // start time
                int y = stoi(line.substr(end + 1, line.size() - (end + 1))); // end time
                N += (y - x + 1);
                anomalies.push_back(pair<int, int>(x, y));
                getline(anomaliesTxt, line);
                getline(anomaliesTxt, line);
            }
            N = Command::info->ts->getData()[0].second.size() - N;
        }
        return anomalies;
    }
    vector<pair<int, int>> NoAnomalies(vector<pair<int, int>> anomalies) {
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
    void  updateFPandTP(int &FP, int &TP, vector<pair<int, int>> *anomalies, vector<pair<int, int>> *unionAnomalies) {
        for (pair<int, int> pCheck: *unionAnomalies) {
            bool flagTP = false;
            for (pair<int, int> pReal: *anomalies) {
                if ((pReal.second >= pCheck.first && pReal.first <= pCheck.first) ||
                    (pCheck.second >= pReal.first && pCheck.second <= pReal.second) ||
                    (pCheck.first <= pReal.first && pCheck.second >= pReal.second) ||
                    (pReal.first <= pCheck.first && pReal.second >= pCheck.second)) {
                    TP += 1;
                    flagTP = true;
                    break;
                }
            }
            if (!flagTP)
                FP += 1;
        }
    }
    void  updateFNorTN(int &P, vector<pair<int, int>> *vector1, vector<pair<int, int>> *vector2) {
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
    vector<pair<int, int>> getUnionReports() {
        vector<AnomalyReport> *reports = Command::info->reports;
        vector<pair<int, int>> unionReports;
        if (reports->empty())
            return unionReports;
        else if (reports->size() == 1) {
            unionReports.push_back(pair<int, int>((*reports)[0].timeStep, (*reports)[0].timeStep));
        }
        AnomalyReport *lastReport = &(*reports)[0];
        int size = reports->size();
        for (int i = 1; i < size; ++i) {
            AnomalyReport report = (*reports)[i];
            if (lastReport->description.compare(report.description) != 0) {
                unionReports.push_back(pair<int, int>(lastReport->timeStep, (*reports)[i - 1].timeStep));
                lastReport = &(*reports)[i];
            } else if (report.timeStep - (*reports)[i-1].timeStep != 1) {
                unionReports.push_back(pair<int, int>(lastReport->timeStep, (*reports)[i - 1].timeStep));
                lastReport = &(*reports)[i];
            }
        }
        unionReports.push_back(pair<int, int>(lastReport->timeStep, (*reports)[reports->size() - 1].timeStep));
        return unionReports;
    }
};

class Exit:public Command {
public:
    Exit(DefaultIO *dio, infoCommand *info): Command(dio) {
        Command::info = info;
    }
    virtual string getDes() {
        string s = "6.exit\n";
        return s;
    }
    virtual void execute() {
        return;
    }
};




#endif /* COMMANDS_H_ */

