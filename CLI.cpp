#include "CLI.h"

CLI::CLI(DefaultIO* dio, vector<Command*> commands) {
    this->dio = dio;
    this->commands = commands;
}

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->commands = vector<Command*>();
}


void CLI::printMenu() {
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    int size = this->commands.size();
    for (int i = 0 ; i < size ; ++i) {
        this->dio->write(this->commands.at(i)->getDes());
    }
}


void CLI::start(){

}


CLI::~CLI() {
}

