#include "../../inc/Server.hpp"

void Server::_mode(int& i, std::vector<std::string>& args)
{
	int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];
    std::string nick = client.getNickname();

	std::vector<std::string>::iterator it;
    for (it = args.begin(); it != args.end(); it++)
    {
        std::cout << "-->" << *it << "<--" << std::endl;
    }

	if (args[1][0] != '#' && args[1][0] != '&')
		return ; //error???

	std::string channelname = args[1];
	if(!_channelExist(channelname))
		return client.sendMessage(ERR_NOSUCHCHANNEL(nick, channelname));
	Channel& channel = this->_channels[channelname];

	if (args.size() == 2) //consult modes
	{
		std::string modestring = channel.getmodeString();

		std::string modeReply = ":localhost " + RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modestring);
    	client.sendMessage(modeReply);
	}

	if (args[2][0] != '-' && args[2][0] != '+')
		return ;//error??


	std::string modes = args[2];
	int j = 3; //problem???

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
			std::string target = args[j];
			++j;
			int targetsock = _findNick(target);
			if (targetsock != -1)
			{
				if (!flagMode)
					channel.removeOperator(targetsock);
				else
					channel.addOperator(targetsock);
			}
			
			break ;
		}

		case 'l':
		{
			int limit = atoi(args[j].c_str());
			++j;

			channel.setMode('l', flagMode);
			if (flagMode)
				channel.setClientLimit(limit);
		}

		default:
			break;
		}
	
	std::string modestring = channel.getmodeString();

	std::string modeReply = ":localhost " + RPL_CHANNELMODEIS(client.getNickname(), channel.getName(), modestring);
    client.sendMessage(modeReply);
	sendMessageToChannel(sock, channel, modeReply);
}