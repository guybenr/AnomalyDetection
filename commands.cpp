//
// Created by adida on 13/12/2021.
//
#include "commands.h"

void UploadCommand::execute() {
    Command::dio->write("Please upload your local train CSV file.");
    const char* path = Command::dio->read().c_str();
    TimeSeries trainTs(path);

}
