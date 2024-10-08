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

// #define DEBUG

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
        std::map<int, Client> _clients; // key in the map is a client's socket
        int _nfds;
        std::map<std::string, Channel> _channels; // key in the map is a channel's name

        void acceptNewClient(void);
        void receiveMessage(int& i);
        std::vector<std::string> _parseBuffer(std::string& str);
        void checkMessage(int& i, std::string& message);
        std::vector<std::string> _parseMessage(std::string& str, char delim);
        void sendMessageToChannel(int cl, Channel& chan, const std::string& message);
        
        // connection messages
        void _pass(int& i, std::vector<std::string>& args);
        void _nick(int&i, std::vector<std::string>& args);
        void _user(int& i, std::vector<std::string>& args);

        // sending messages
        void _privmsg(int& i, std::vector<std::string>& args);

        // channel operations
        void _join(int& i, std::vector<std::string>& args);
        void _kick(int& i, std::vector<std::string>& args);
        void _invite();
        void _topic(int& i, std::vector<std::string>& args);
        void _mode();

        // help functions
        void _ping(int& i, std::vector<std::string>& args);
        bool _nickExist(std::string& nick);
        bool _channelExist(std::string& channel);
        void _rplNamesList(int client, std::string& channel, std::vector<int>& socks);
        int  _findNick(std::string& nick);
        bool _validChannelName(std::string& name);
        bool _validNick(std::string& nick);

        // void _removeClntFromChan(int cltsock, std::string& channelname);
        
};

#endif