
#include "Server.h"

Server::Server(int port)throw (const char*) {
    int sockfd = socket(domain, type, protocol);
    int setsockopt(int sockfd, int level, int optname,
                   const void *optval, socklen_t optlen);
    int bind(int sockfd, const struct sockaddr *addr,
             socklen_t addrlen);
    int listen(int sockfd, int backlog);
    int new_socket= accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
}

void Server::start(ClientHandler& ch)throw(const char*){
}

void Server::stop(){
    t->join(); // do not delete this!
}

Server::~Server() {
}

