#include "../../inc/Server.hpp"

void Server::_nick(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    
    if (args.size() < 2)
    {
        this->_clients[sock].sendMessage(ERR_NONICKNAMEGIVEN(this->_clients[sock].getNickname()));
        return;
    }
    std::string nick = args[1];
    if (nick == "")
    {
        this->_clients[sock].sendMessage(ERR_NONICKNAMEGIVEN(this->_clients[sock].getNickname()));
        return;
    }
    for (size_t i = 2; i < args.size(); i ++)
    {
        nick += " ";
        nick += args[i];
    }
    if (_nickExist(nick))
    {
        this->_clients[sock].sendMessage(ERR_NICKNAMEINUSE(this->_clients[sock].getNickname(), nick));
        return;
    }
    if (!_validNick(nick))
    {
        this->_clients[sock].sendMessage(ERR_ERRONEUSNICKNAME(this->_clients[sock].getNickname(), nick));
        return;
    }
    std::string old_nick = this->_clients[sock].getNickname();
    this->_clients[sock].setNickname(nick);
    std::string msg = ":" + old_nick + " NICK " + nick;
    this->_clients[sock].sendMessage(msg);
    // do we need to inform others?

    if (this->_clients[sock].isRegistered() || this->_clients[sock].getUsername() == "")
        return;

    this->_clients[sock].setRegisteredStatus(true);
    this->_clients[sock].sendMessage(RPL_WELCOME(nick));
    this->_clients[sock].sendMessage(RPL_YOURHOST(nick));
    this->_clients[sock].sendMessage(RPL_CREATED(nick));
    this->_clients[sock].sendMessage(RPL_MYINFO(nick));
}

bool Server::_nickExist(std::string& nick)
{
    if (nick == "TheBot")
        return (true);
    for (int i = 1; i < this->_nfds; i ++)
    {
        int sock = this->_fds[i].fd;
        if (this->_clients[sock].getNickname() == nick)
            return (true);
    }
    return (false);
}

bool Server::_validNick(std::string& nick)
{
    if (nick[0] == '$' || nick[0] == ':' || nick[0] == '&' || nick[0] == '#')
        return (false);
    for (size_t i = 0; i < nick.length(); i ++)
    {
        if (nick[i] == ',' || nick[i] == '*' || nick[i] == '!' || nick[i] == '?' || nick[i] == '@' || nick[i] == '.' || nick[i] == ' ')
            return (false);
    }
    return (true);
}