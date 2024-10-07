#include "../../inc/Server.hpp"

void Server::_topic(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    Client client = this->_clients[sock];
    std::string nick = client.getNickname();

    std::string channelname = args[1];
    Channel channel;

    // std::vector<std::string>::iterator it;

    // for (it = args.begin(); it != args.end(); it++)
    // {
    //     std::cout << "->" << *it << "<-" << std::endl;
    // }
    
    if (!_channelExist(channelname))
        return client.sendMessage(ERR_NOSUCHCHANNEL(nick, channelname));
    channel = _channels[args[1]];

    if (!channel.isClientInChannel(sock))
        return client.sendMessage(ERR_NOTONCHANNEL(nick, channelname));

    std::string newTopic = args[2];
    if (newTopic.empty())
    {
        if (channel.getTopic() == "")
            client.sendMessage(RPL_NOTOPIC(nick, channelname));
        else
            client.sendMessage(RPL_TOPIC(nick, channelname, channel.getTopic()));
    }
    else
    {
        if (channel.getMode('t') && !channel.isOperator(sock))
            return ; // Any error????
        else if (newTopic == ":" || newTopic == " ")
            channel.setTopic("");
        else
            channel.setTopic(newTopic.substr(1, newTopic.size()));

        sendMessageToChannel(sock, channel, RPL_TOPIC(nick, channelname, channel.getTopic()));
        client.sendMessage(RPL_TOPIC(nick, channelname, channel.getTopic()));
    }
}