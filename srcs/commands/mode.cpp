#include "../../inc/Server.hpp"

void Server::_mode(int& i, std::vector<std::string>& args)
{
	int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];
    std::string nick = client.getNickname();

	if (args.size() < 2)
		return client.sendMessage(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"));

	if (args[1][0] != '#' && args[1][0] != '&')
		return ;

	std::string channelname = args[1];
	if(!_channelExist(channelname))
		return client.sendMessage(ERR_NOSUCHCHANNEL(nick, channelname));
	Channel& channel = this->_channels[channelname];

	if (args.size() <= 2) //consult modes
	{
		std::string modestring = channel.getmodeString();

		std::string modeReply = ":localhost " + RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modestring);
    	return client.sendMessage(modeReply);
	}

	if (!channel.isOperator(sock))
		return client.sendMessage(ERR_CHANOPRIVSNEEDED(nick, channelname));

	if (args[2][0] != '-' && args[2][0] != '+')
		return ;

	std::string modes = args[2];
	size_t j = 3;

	bool		flagMode = false;
	while (modes.empty() == false)
	{
		char mode = modes[0];
		modes.erase(0,1);

		switch (mode)
		{
		case '+':
			flagMode = true;
			break ;

		case '-':
			flagMode = false;
			break ;
		
		case 'i':
			channel.setMode('i', flagMode);
			break ;

		case 't':
			channel.setMode('t', flagMode);
			break ;

		case 'k':
		{
			if (j >= args.size() && flagMode)
				break ;
			std::string key = args[j];
			++j;

			channel.setMode('k', flagMode);
			if (flagMode)
				channel.setKey(key);
			// else
			// 	channel.setKey(key);
			break ;
		}

		case 'o':
		{
			if (j >= args.size() && flagMode)
				break ;
			std::string target = args[j];
			++j;
			int targetsock = _findNick(target);
			if (targetsock != -1)
			{
				if (!flagMode)
				{
					channel.removeOperator(targetsock);
					this->_clients[targetsock].setAdmin(channelname, false);
				}
				else
				{
					channel.addOperator(targetsock);
					this->_clients[targetsock].setAdmin(channelname, true);
				}
			}
			break ;
		}

		case 'l':
		{
			if (j >= args.size() && flagMode)
				break ;
			int limit = atoi(args[j].c_str());
			if (limit > CLIENT_LIMIT)
				limit = CLIENT_LIMIT;
			++j;

			channel.setMode('l', flagMode);
			if (flagMode)
				channel.setClientLimit(limit);
			else
				channel.setClientLimit(CLIENT_LIMIT);
			break ;
		}

		default:
			break;
		}
	}
	
	std::string modestring = channel.getmodeString();

	std::string modeReply = ":localhost " + RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modestring);
    client.sendMessage(modeReply);
	sendMessageToChannel(sock, channel, modeReply);
}