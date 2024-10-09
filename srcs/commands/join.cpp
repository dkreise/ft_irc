#include "../../inc/Server.hpp"

void Server::_join(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];
    std::vector<std::string> channels;
    std::vector<std::string> keys;
    Channel channel;
    std::vector<int> fds;

    if (args.size() < 2)
    {
        client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "JOIN"));
        return;
    }
    bool are_keys = args.size() > 2 ? true : false;
    channels = _parseMessage(args[1], ',');
    if (are_keys)
    {
        keys = _parseMessage(args[2], ',');
    }
    int n = channels.size();
    for (int i = 0; i < n; i ++)
    {
        std::string chan_name = channels[i];
        if (_channelExist(chan_name))
        {
            std::cout << "Channel exists\n";
            channel = this->_channels[chan_name];

            if (channel.getMode('i') && )
            {
                client.sendMessage(ERR_INVITEONLYCHAN(client.getNickname(), chan_name));
                return;
            }
            // check if too many channels for client
            if (client.getChannelCnt() >= client.getChannelLim())
            {
                client.sendMessage(ERR_TOOMANYCHANNELS(client.getNickname(), chan_name));
                return;
            }
            // check if too many clients in channel
            // std::cout << "channel count-->" << channel.getClientCnt() << std::endl;
            if (channel.getClientCnt() >= channel.getClientLim())
            {
                client.sendMessage(ERR_CHANNELISFULL(client.getNickname(), chan_name));
                return;
            }
            // check the key if it is required for channel
            if (channel.getMode('k'))
            {
                if (!are_keys || i >= (int)keys.size() || keys[i] != channel.getKey())
                {
                    client.sendMessage(ERR_BADCHANNELKEY(client.getNickname(), chan_name));
                    return;
                }
            }
#ifdef DEBUG
            std::cout << "SOCK TO PUSH::" << sock << std::endl;
#endif            
            // this->_channels[chan_name].addClient(sock);
            this->_channels[chan_name].addClient(sock);
            this->_clients[sock].addChannel(chan_name, false);
            fds = this->_channels[chan_name].getClients();

            client.sendMessage(RPL_TOPIC(client.getNickname(), chan_name, channel.getTopic()));
        }
        else
        {
#ifdef DEBUG
            std::cout << "Channel does not exist, creating a new one\n";
#endif
            if (client.getChannelCnt() >= client.getChannelLim())
            {
                client.sendMessage(ERR_TOOMANYCHANNELS(client.getNickname(), chan_name));
                return;
            }
            // check for first char and valid chars:
            if (!_validChannelName(chan_name))
            {
                client.sendMessage(ERR_BADCHANMASK(chan_name));
                return;
            }
            Channel new_channel(chan_name);
            new_channel.addClient(sock);
            new_channel.addOperator(sock);
            if (are_keys && i < (int)keys.size())
            {
                new_channel.setKey(keys[i]);
                new_channel.setMode('k', true);
            }
            this->_channels[chan_name] = new_channel;
            this->_clients[sock].addChannel(chan_name, true);
            fds = new_channel.getClients();
            channel = new_channel;
        }
#ifdef DEBUG
        std::cout << "FDS:: ";
        for (size_t i = 0; i < fds.size(); i++)
        {
            std::cout << fds[i] << ", ";
        }
        std::cout << std::endl;
#endif
        sendMessageToChannel(sock, channel, RPL_JOIN(client.getNickname(), client.getRealname(), client.getHostname(), chan_name)); // or sendChannel ?
        if (channel.getMode('t'))
        {
            client.sendMessage(RPL_TOPIC(client.getNickname(), chan_name, channel.getTopic()));
        }
        // RPL_NAMREPLY, RPL_ENDOFNAMES:
        _rplNamesList(sock, chan_name, fds);
        fds.clear();
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

void Server::_rplNamesList(int client, std::string& channel, std::vector<int>& socks)
{
    int n = socks.size();
    std::string nick;
    std::string userlist("");

    std::string client_nick = this->_clients[client].getNickname();
    
    for (int i = 0; i < n; i ++)
    {
        nick = this->_clients[socks[i]].getNickname();

        if (this->_clients[socks[i]].isAdmin(channel))
            userlist += "@" + nick + " ";
        else
            userlist += nick + " ";
    }
    userlist += "@TheBot";
    this->_clients[client].sendMessage(RPL_NAMREPLY(client_nick, channel, userlist));
    this->_clients[client].sendMessage(RPL_ENDOFNAMES(client_nick, channel));
}

bool Server::_validChannelName(std::string& name)
{
    if (name[0] != '#' && name[0] != '&')
    {
        return (false);
    }
    // some other chars? ','??
    return (true);
}