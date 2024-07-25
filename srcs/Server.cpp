#include "../inc/Server.hpp"

Server::Server(void) {}

Server::Server(int port) : _port(port) {}

void Server::start(void)
{
    this->_sock = socket(AF_INET, SOCK_STREAM, 0); // protect
    
    struct sockaddr_in info;
    info.sin_family = AF_INET;
    info.sin_port = htons(this->_port);
    info.sin_addr.s_addr = INADDR_ANY;

    bind(this->_sock, (struct sockaddr*) &info, sizeof(info)); // protect
    listen(this->_sock, SOMAXCONN); // protect
}