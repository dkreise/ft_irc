#include "../../inc/Server.hpp"

void Server::_kick(int& i, std::vector<std::string>& args)
{
	int sock = this->_fds[i].fd;
    Client client = this->_clients[sock];
    std::string nick = client.getNickname();
	std::string kickMsg = "No reason";

	// std::vector<std::string>::iterator it;
    // for (it = args.begin(); it != args.end(); it++)
    // {
    //     std::cout << "-->" << *it << "<--" << std::endl;
    // }

	if (args.size() < 2) //3??
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
	// for (std::vector<std::string>::iterator i = targets.begin(); i != targets.end(); i++)
	// {
	// 	std::cout << "target -->" << *i << std::endl;
	// }
	
	for (size_t j = 0; j < targets.size(); j++)
	{
		int		targetSock = _findNick(targets[j]);
		Client	targetClnt = _clients[targetSock];

		if (!_nickExist(targets[j]))
			return client.sendMessage(ERR_NOSUCHNICK(nick, targets[j].c_str()));
		if (!channel.isClientInChannel(targetSock))
			return client.sendMessage(ERR_USERNOTINCHANNEL(nick, targets[j].c_str(), channelname));

		std::string msg = KICK_MSG(nick, client.getUsername(), client.getHostname(), channelname, targetClnt.getNickname(), kickMsg);
		sendMessageToChannel(targetSock, channel, msg);
		targetClnt.sendMessage(msg);// i think its okay??

		targetClnt.removeFromChan(channelname);
		channel.removeClient(targetSock);

		channel.removeInvited(targetSock);
	}
	// if (channel is empty)
	// 	erase?
	
}