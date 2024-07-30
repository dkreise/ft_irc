#include "Server.hpp"

void Server::_join(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;

    if (args.size() == 1)
    {
        std::cout << ERR_NEEDMOREPARAMS;
        return;
    }
    bool are_keys = args.size() > 2 ? true : false;
    std::vector<std::string> channels = _parseMessage(args[1], ',');
    if (are_keys)
    {
        std::vector<std::string> keys = _parseMessage(args[2], ',');
    }
    int n = channels.size();
    for (int i = 0; i < n; i ++)
    {
        if (_channelExist(channels[i]))
        {
#ifdef DEBUG
            std::cout << "Channel exists\n";
#endif
            // check for invite only
            // check if too many channels for client
            // check if too many clients in channel
            // check the key if it is required for channel
            this->_channels[channels[i]].addClient(sock);
            this->_clients[sock].addChannel(channels[i]);
        }
        else
        {
#ifdef DEBUG
            std::cout << "Channel does not exist, creating a new one\n";
#endif
            // check for first char and valid chars
            Channel new_channel(channels[i]);
            new_channel.addClient(sock);
            // add clients rights ?
            this->_channels[channels[i]] = new_channel;
        }
    }
}

bool Server::_channelExist(std::string& channel)
{  
    std::map<std::string, Channel>::iterator it;
    for (it = this->_channels.begin(); it != this->_channels.end(); it ++)
    {
        if (it->first == channel)
            return (true);
    }
    return (false);
}