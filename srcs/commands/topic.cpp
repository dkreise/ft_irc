#include "../../inc/Server.hpp"

void Server::_topic(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    Client client = this->_clients[sock];
    std::string nick = client.getNickname();

    std::string channelname = args[1];

    if (args.size() < 2)
        return client.sendMessage(ERR_NEEDMOREPARAMS(nick, "TOPIC"));

    if (!_channelExist(channelname))
        return client.sendMessage(ERR_NOSUCHCHANNEL(nick, channelname));
    Channel& channel = _channels[args[1]];

    if (!channel.isClientInChannel(sock))
        return client.sendMessage(ERR_NOTONCHANNEL(nick, channelname));

    if (args.size() < 3)
    {
        if (channel.getTopic() == "")
            client.sendMessage(RPL_NOTOPIC(nick, channelname));
        else
            client.sendMessage(RPL_TOPIC(nick, channelname, channel.getTopic()));
    }
    else
    {
        std::string newTopic = args[2];
        for (size_t i = 3; i < args.size(); i ++)
        {
            newTopic += " ";
            newTopic += args[i];
        }
        if (channel.getMode('t') && !channel.isOperator(sock))
            return client.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channelname)); 
        else if (newTopic == ":")
            channel.setTopic("");
        else
            channel.setTopic(newTopic.substr(0, newTopic.size())); //Check in nc

        sendMessageToChannel(sock, channel, RPL_TOPIC(nick, channelname, channel.getTopic()));
        client.sendMessage(RPL_TOPIC(nick, channelname, channel.getTopic()));
    }
}