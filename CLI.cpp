#include "CLI.h"

CLI::CLI(DefaultIO* dio, vector<Command*> commands) {
    this->dio = dio;
    this->commands = commands;
}

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->commands = vector<Command*>();
    struct infoCommand* info = new infoCommand();
    this->commands.push_back(new UploadCommand(dio, info));
    this->commands.push_back(new AlgorithmSettings(dio, info));
    this->commands.push_back(new DetectAnomalies(dio, info));
    this->commands.push_back(new Display(dio, info));
    this->commands.push_back(new Analyze(dio, info));
    this->commands.push_back(new Exit(dio, info));
}


void CLI::printMenu() {
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    for (Command * command : this->commands) {
        this->dio->write(command->getDes());
    }
}


void CLI::start(){
    do {
        this->printMenu();
        float index = 0;
        this->dio->read(&index);
        int i = (int) index;
        if (i >= 6 || i <= 0) {
            break;
        }
        this->commands[i-1]->execute();
    } while(true);
}


CLI::~CLI() {
    this->commands.clear();
    delete this->dio;
}