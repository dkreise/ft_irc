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
    if (_nickExist(nick))
    {
        //std::cout << ERR_NICKNAMEINUSE;
        this->_clients[sock].sendMessage
        return;
    }
    // check for invalid chars
    std::string old_nick = this->_clients[sock].getNickname();
    this->_clients[sock].setNickname(nick);
    // send to users not server:
    printf(":%s NICK %s\n", old_nick.c_str(), nick.c_str());
}

bool Server::_nickExist(std::string& nick)
{
    for (int i = 1; i < this->_nfds; i ++)
    {
        int sock = this->_fds[i].fd;
        if (this->_clients[sock].getNickname() == nick)
            return (true);
    }
    return (false);
}