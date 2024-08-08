#include "../../inc/Server.hpp"

void Server::_privmsg(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    Client client = this->_clients[sock];
    std::vector<std::string> targets;
    std::string text_to_send;

    if (args.size() < 2)
    {
        client.sendMessage(ERR_NORECIPIENT(client.getNickname(), "PRIVMSG"));
        return;
    }
    targets = _parseMessage(args[1], ',');
    
    if (args.size() < 3)
    {
        client.sendMessage(ERR_NOTEXTTOSEND(client.getNickname()));
        return;
    }
    text_to_send = args[2];

    for (size_t i = 0; i < targets.size(); i ++)
    {
        if (targets[i][0] == '#' || targets[i][0] == '&')
        {
            // (can be prefixed ??)
            // send to channel
        }
        else if (_nickExist(targets[i]))
        {
            // send to client
        }
    }
}