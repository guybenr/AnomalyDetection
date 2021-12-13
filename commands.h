

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
    virtual ~DefaultIO(){}

    // you may add additional methods here
};

// you may add here helper classes

struct infoCommand {
    SimpleAnomalyDetector *detector = nullptr;
    TimeSeries *ts = nullptr;
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
    virtual ~Command(){}
};

// implement here your command classes

class UploadCommand:public Command {
    UploadCommand(DefaultIO *dio, infoCommand *info);
    virtual void execute();
};


#endif /* COMMANDS_H_ */
