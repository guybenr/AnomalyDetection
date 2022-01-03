/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <thread>
#include "commands.h"
#include "CLI.h"

#include <bits/sockaddr.h>
#include <netinet/in.h>

using namespace std;

// edit your ClientHandler interface here:
class ClientHandler{
public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
class AnomalyDetectionHandler:public ClientHandler{
public:
    virtual void handle(int clientID){
        DefaultIO* dio = new SocketIO(clientID);
        CLI *cli = new CLI(dio);
        cli->start();
        delete cli;
    }
};


// implement on Server.cpp
class Server {
    bool forceStop = false;
    int clientLimit = 5;
    thread* t; // the thread to run the start() method in
    int fd;
    sockaddr_in server;
    sockaddr_in client;
    // you may add data members

public:
    Server(int port) throw (const char*);
    virtual ~Server();
    void start(ClientHandler& ch)throw(const char*);
    void stop();
};

#endif /* SERVER_H_ */
