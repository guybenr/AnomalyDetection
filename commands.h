#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

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
        while (row != "done") {
            file << row;
            row = this->read();
        }
        file.close();
    }
    void readFromFile(string path) {
        std::ifstream file;
        file.open(path);
        string row;
        getline(file, row);
        while (row != "done") {
            this->write(row);
            getline(file, row);
        }
        file.close();
    }
    virtual ~DefaultIO(){}

    // you may add additional methods here
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
    virtual ~Command()=0;
};

// implement here your command classes

class UploadCommand:public Command {
public:
    UploadCommand(DefaultIO *dio, infoCommand *info);
    virtual string getDes();
    virtual void execute();
};

class AlgorithmSettings:public Command {
public:
    AlgorithmSettings(DefaultIO *dio, infoCommand *info);
    virtual string getDes();
    virtual void execute();
};


class DetectAnomalies:public Command {
public:
    DetectAnomalies(DefaultIO *dio, infoCommand *info);
    virtual string getDes();
    virtual void execute();
};


class Display:public Command {
public:
    Display(DefaultIO *dio, infoCommand *info);
    virtual string getDes();
    virtual void execute();
};

class Analyze:public Command {
public:
    Analyze(DefaultIO *dio, infoCommand *info);
    virtual string getDes();
    virtual void execute();

private:
    vector<pair<int, int>> updateAnomalies(int &P, int &N, std::ifstream &anomaliesTxt);
    vector<pair<int, int>> NoAnomalies(vector<pair<int, int>> anomalies);
    void  updateFPandTP(int &FP, int &TP, vector<pair<int, int>> *anomalies, vector<pair<int, int>> *unionAnomalies);
    void  updateFNorTN(int &P, vector<pair<int, int>> *vector1, vector<pair<int, int>> *vector2);
    vector<pair<int, int>> getUnionReports();

};


class Exit:public Command {
public:
    Exit(DefaultIO *dio, infoCommand *info);
    virtual string getDes();
    virtual void execute();
};
#endif /* COMMANDS_H_ */