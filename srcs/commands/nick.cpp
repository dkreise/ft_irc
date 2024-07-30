#include "Server.hpp"

void Server::_nick(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    
    if (args.size() < 2)
    {
        std::cout << ERR_NONICKNAMEGIVEN;
        return;
    }
    std::string nick = args[1];
    if (_nickIsUsed(nick))
    {
        std::cout << ERR_NICKNAMEINUSE;
        return;
    }
    // check for invalid chars
    std::string old_nick = this->_clients[sock].getNickname();
    this->_clients[sock].setNickname(nick);
    // send to users not server:
    printf(":%s NICK %s\n", old_nick.c_str(), nick.c_str());
}

bool Server::_nickIsUsed(std::string& nick)
{
    for (int i = 1; i < this->_nfds; i ++)
    {
        int sock = this->_fds[i].fd;
        if (this->_clients[sock].getNickname() == nick)
            return (true);
    }
    return (false);
}