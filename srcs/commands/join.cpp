#include "../../inc/Server.hpp"

void Server::_join(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;

    if (args.size() < 2)
    {
        this->_clients[sock].sendMessage(ERR_NEEDMOREPARAMS);
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
        std::string chan_name = channels[i];
        if (_channelExist(chan_name))
        {
#ifdef DEBUG
            std::cout << "Channel exists\n";
#endif
            // check for invite only:
            if (this->_channels[chan_name].getMode('i'))
                this->_clients[sock].sendMessage(ERR_INVITEONLYCHAN(this->_clients[sock].getNickname(), chan_name));
            // check if too many channels for client
            
            // check if too many clients in channel
            // check the key if it is required for channel
            this->_channels[chan_name].addClient(sock);
            this->_clients[sock].addChannel(chan_name);
            // show channel topic ?
        }
        else
        {
#ifdef DEBUG
            std::cout << "Channel does not exist, creating a new one\n";
#endif
            // check for first char and valid chars
            Channel new_channel(chan_name);
            new_channel.addClient(sock);
            // add clients rights ?
            this->_channels[chan_name] = new_channel;
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