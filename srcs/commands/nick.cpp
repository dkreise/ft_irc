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
    for (size_t j = 2; j < args.size(); j ++)
    {
        nick += " ";
        nick += args[j];
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

    if (old_nick != "")
    {
        for (int j = 0; j < this->_nfds; j ++)
        {
            int cur = this->_fds[j].fd;
            if (cur != this->_sock && cur != sock)
            {
                this->_clients[cur].sendMessage(msg);
            }
        }
    }

    if (this->_clients[sock].isRegistered() || this->_clients[sock].getUsername() == "")
        return;

    this->_clients[sock].setRegisteredStatus(true);
    this->_clients[sock].sendMessage(RPL_WELCOME(nick));
    this->_clients[sock].sendMessage(RPL_YOURHOST(nick));
    this->_clients[sock].sendMessage(RPL_CREATED(nick));
    this->_clients[sock].sendMessage(RPL_MYINFO(nick));
    msg = ":TheBot PRIVMSG " + nick + ": Welcome to our beautiful Server! I am your bot that does nothing!";
    this->_clients[sock].sendMessage(msg);
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