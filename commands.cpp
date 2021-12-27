
#include "commands.h"
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

string SocketIO::read() {
    vector<char> buf(5000);

}

void SocketIO::write(string text) {
    send(this->socketID,text.c_str(),text.size(),MSG_CONFIRM);
}

void SocketIO::read(float *f) {

}

void SocketIO::write(float f) {
    send(this->socketID, &f, sizeof(f), 0);
}
