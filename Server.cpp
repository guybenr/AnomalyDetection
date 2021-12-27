
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


Server::Server(int port)throw (const char*) {
    sockaddr_in sockAddrIn;
    in_addr inAddr;
    inAddr.s_addr = INADDR_ANY;
    sockAddrIn.sin_family = AF_INET;
    sockAddrIn.sin_port = htons(port);
    sockAddrIn.sin_addr = inAddr;
    sockaddr *sockAddr = (struct sockaddr*)&sockAddrIn;
    int sock=0,b=0,l=0;
    socklen_t size = sizeof(sockAddr);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("error socket");
    }
    if ((b = bind(sock, sockAddr, size)) == 0) {
        throw std::runtime_error("error bind");
    }
    if ((l = listen(sock, 5)) == 0) {
        throw std::runtime_error("error listen");
    }
    int new_socket = accept(sock, sockAddr, &size);
}

void Server::start(ClientHandler& ch)throw(const char*){
}

void Server::stop(){
    t->join(); // do not delete this!
}

Server::~Server() {
}

