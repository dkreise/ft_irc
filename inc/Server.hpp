#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include "Client.hpp"

#define DEBUG

class Client;

class Server
{
    public:
        Server(int port, std::string password);

        void doPollLoop(void);
        
    private:
        int _port;
        std::string _password;
        int _sock;
        struct sockaddr_in _info;
        std::vector<struct pollfd> _fds;
        std::map<int, Client> _clients;
        int _nfds;

        void acceptNewClient(void);
        void receiveMessage(int& i);
        std::vector<std::string> _parseBuffer(std::string& str);
        void checkMessage(int& i, std::string& message);
        std::vector<std::string> _parseMessage(std::string& str, char delim);
        
        void _pass(int& i, std::vector<std::string>& args);
        
};

#endif