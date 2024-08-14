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
            if (_channelExist(targets[i]))
            {
                // check if it can be sent ?
                sendMessageToChannel(sock, this->_channels[targets[i]], text_to_send);
            }
            else
            {
                client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), targets[i]));
                return;
            }
        }
        else if (_nickExist(targets[i]))
        {
            int sock_target = _findNick(targets[i]);
            this->_clients[sock_target].sendMessage(text_to_send);
        }
        else
        {
            client.sendMessage(ERR_NOSUCHNICK(client.getNickname(), targets[i]));
            return;
        }
    }
}

int Server::_findNick(std::string& nick)
{
    for (int i = 1; i < this->_nfds; i ++)
    {
        int sock = this->_fds[i].fd;
        if (this->_clients[sock].getNickname() == nick)
            return (sock);
    }
    return (-1);
}