#include "../../inc/Server.hpp"

void Server::_kick(int& i, std::vector<std::string>& args)
{
	int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];
    std::string nick = client.getNickname();
	std::string kickMsg = "No reason";

	if (args.size() < 2)
        return client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "KICK"));
	if (args.size() >= 4)
		kickMsg = args[3];
	std::string channelname = args[1];
	if(!_channelExist(channelname))
		return client.sendMessage(ERR_NOSUCHCHANNEL(nick, channelname));
	
	Channel& channel = this->_channels[channelname];
	if (!channel.isClientInChannel(sock))
		return client.sendMessage(ERR_NOTONCHANNEL(nick, channelname));
	if (!channel.isOperator(sock))
		return client.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channelname));

	std::vector<std::string> targets = _parseMessage(args[2], ',');
	
	for (size_t j = 0; j < targets.size(); j++)
	{
		int		targetSock = _findNick(targets[j]);
		Client	targetClnt = _clients[targetSock];

		if (targets[j] == "TheBot")
		{
			std::string aux = ":TheBot PRIVMSG " + client.getNickname() + ": You cannot kick me!!..";
            client.sendMessage(aux);
			sendMessageToChannel(-1, channel, aux);
			continue;
		}
		if (!_nickExist(targets[j]))
		{
			client.sendMessage(ERR_NOSUCHNICK(nick, targets[j].c_str()));
			continue;
		}
		if (!channel.isClientInChannel(targetSock))
		{
			client.sendMessage(ERR_USERNOTINCHANNEL(nick, targets[j].c_str(), channelname));
			continue;
		}
		std::string msg = KICK_MSG(nick, client.getUsername(), client.getHostname(), channelname, targetClnt.getNickname(), kickMsg);
		sendMessageToChannel(targetSock, channel, msg);
		targetClnt.sendMessage(msg);

		targetClnt.removeFromChan(channelname);
		channel.removeClient(targetSock);

		channel.removeInvited(targetSock);
	}
	if (channel.isEmpty())
		this->_channels.erase(channelname);
}