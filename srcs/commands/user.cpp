#include "../../inc/Server.hpp"

void Server::_user(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    std::string nick = this->_clients[sock].getNickname();

    if (this->_clients[sock].isRegistered())
    {
        this->_clients[sock].sendMessage(ERR_ALREADYREGISTERED(nick));
        return;
    }
    else if (args.size() < 5 || args[1] == "")
    {
        this->_clients[sock].sendMessage(ERR_NEEDMOREPARAMS(nick, "USER"));
        return;
    }
    
    std::string username = args[1];
    std::string hostname = args[2];
    std::string realname = args[4];
    for (size_t i = 5; i < args.size(); i ++)
    {
        realname += " ";
        realname += args[i];
    }

    this->_clients[sock].setUsername(username);
    this->_clients[sock].setHostname(hostname);
    this->_clients[sock].setRealname(realname);
    if (nick == "")
    {
        return;
    }
    this->_clients[sock].setRegisteredStatus(true);
    this->_clients[sock].sendMessage(RPL_WELCOME(nick));
    this->_clients[sock].sendMessage(RPL_YOURHOST(nick));
    this->_clients[sock].sendMessage(RPL_CREATED(nick));
    this->_clients[sock].sendMessage(RPL_MYINFO(nick));
}