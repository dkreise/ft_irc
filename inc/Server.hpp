#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdio>
#include <string.h>
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <map>
#include <algorithm>
#include <string>
#include <sstream>
#include "Client.hpp"
#include "Channel.hpp"
#include "Errors.hpp"

#define DEBUG

class Client;
class Channel;

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
        std::vector<Channel> _channels;

        void acceptNewClient(void);
        void receiveMessage(int& i);
        std::vector<std::string> _parseBuffer(std::string& str);
        void checkMessage(int& i, std::string& message);
        std::vector<std::string> _parseMessage(std::string& str, char delim);
        
        // connection messages
        void _pass(int& i, std::vector<std::string>& args);
        void _nick(int&i, std::vector<std::string>& args);
        void _user(int& i, std::vector<std::string>& args);

        // channel operations
        void _join();
        void _kick();
        void _invite();
        void _topic();
        void _mode();

        // help functions
        bool _nickIsUsed(std::string& nick);
        
};

#endif