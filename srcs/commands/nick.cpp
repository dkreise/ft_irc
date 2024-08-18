#include "../../inc/Server.hpp"

/*
MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
SHOULD NOT contain any dot character ('.', 0x2E).
*/

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