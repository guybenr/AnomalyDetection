
#include "Server.h"
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "Server.h"
#include "signal.h"

bool flag = true;

Server::Server(int port)throw (const char*) {
    this->server.sin_family = AF_INET;
    this->server.sin_port = htons(port);
    this->server.sin_addr.s_addr = INADDR_ANY;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw std::runtime_error("socket failed");
    }
    if (bind(fd, (struct sockaddr*)&this->server, sizeof(server)) < 0) {
        throw std::runtime_error("bind failed");
    }
    if (listen(fd, this->clientLimit) < 0) {
        throw std::runtime_error("listen failed");
    }
    this->fd = fd;
}

void Server::start(ClientHandler& ch)throw(const char*){
    t = new thread([&ch, this](){
        while(!forceStop) {
            socklen_t clientSize = sizeof(this->client);
            timeval timeout = {0, 200};
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(this->fd, &fds);
            select(this->fd + 1, &fds, NULL, NULL, &timeout);
            if(!FD_ISSET(this->fd, &fds)) {
                continue;
            }
            int clientFd = accept(this->fd, (struct sockaddr *) &client, &clientSize);
            if (clientFd > 0) {
                ch.handle(clientFd);
                close(clientFd);
            } else
                throw "Connection Failed";
        }
        close(fd);
    });
}

void Server::stop(){
    this->forceStop = true;
    t->join(); // do not delete this!
    delete t;
}

Server::~Server() = default;

