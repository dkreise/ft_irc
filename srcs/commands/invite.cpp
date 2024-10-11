#include "../../inc/Server.hpp"

void Server::_invite(int& i, std::vector<std::string>& args)
{
	int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];
    std::string nick = client.getNickname();

	if (args.size() < 2)
        return client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "INVITE"));
	
	std::string targetnick = args[1];
	std::string channelname = args[2];

	if (!_channelExist(channelname))
		return client.sendMessage(ERR_NOSUCHCHANNEL(nick, channelname));
	Channel& channel = this->_channels[channelname];

	if (!channel.isClientInChannel(sock))
		return client.sendMessage(ERR_NOTONCHANNEL(nick, channelname));

	if (!channel.isOperator(sock))
		return client.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channelname));

	int targetsock = _findNick(targetnick);
	if (targetsock == -1)
		return client.sendMessage(ERR_NOSUCHNICK(nick, targetnick));
	
	if (channel.isClientInChannel(targetsock))
		return client.sendMessage(ERR_USERONCHANNEL(nick, targetnick, channelname));

	client.sendMessage(RPL_INVITING(nick, targetnick, channelname));

	Client& target = _clients[targetsock];

	channel.addInvited(targetsock);
	std::string targetMsg = nick + " INVITE " + targetnick + " " + channelname;
	target.sendMessage(targetMsg);

}