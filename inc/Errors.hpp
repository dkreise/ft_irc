#ifndef ERRORS_HPP
#define ERRORS_HPP

// pass
#define ERR_PASSWDMISMATCH(client) (std::string("464 ") + client + " :Password incorrect")
#define ERR_NEEDMOREPARAMS(client, cmd) (std::string("461 ") + client + " " + cmd + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(client) (std::string("462 ") + client + " :You may not reregister")

// nick
#define ERR_NONICKNAMEGIVEN(client) (std::string("431 ") + client + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(client, nick) (std::string("432 ") + client + " " + nick + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(client, nick) (std::string("433 ") + client + " " + nick + " :Nickname  is already in use")

// join
#define ERR_NOSUCHCHANNEL(client, channel) (std::string("403 ") + client + " " + channel + " :No such channel exists")
#define ERR_TOOMANYCHANNELS(client, channel) (std::string("405 ") + client + " " + channel + " :You have joined too many channels")
#define ERR_BADCHANNELKEY(client, channel) (std::string("475 ") + client + " " + channel + " :Cannot join channel (+k)")
#define ERR_BANNEDFROMCHAN(client, channel) (std::string("474 ") + client + " " + channel + " :Cannot join channel (+b)")
#define ERR_CHANNELISFULL(client, channel) (std::string("471 ") + client + " " + channel + " :Cannot join channel (+l)")
#define ERR_BADCHANMASK(channel) (std::string("476 ") + channel + " :Bad Channel Mask")
#define RPL_JOIN(client, name, host, channel) (std::string(":") + client + "!" + name + "@" + host + " JOIN " + channel)
#define RPL_NAMREPLY(client, channel, userlist) (std::string(":localhost 353 ") + client + " = " + channel + " :" + userlist)
#define RPL_ENDOFNAMES(client, channel) (std::string(":localhost 366 ") + client + " " + channel + " :End of /NAMES list")

// privmsg
#define ERR_CANNOTSENDTOCHAN(client, channel) (std::string("localhost 404 " + client + " " + channel + " :Cannot send to channel"))
#define ERR_NORECIPIENT(client, command) (std::string("411 " + client + " :No recipient given (" + command + ")"))
#define ERR_NOTEXTTOSEND(client) (std::string("412 " + client + " :No text to send"))
#define ERR_NOSUCHNICK(client, nickname) (std::string("401 ") + client + " " + nickname + " :No such nick/channel")

// registration
#define RPL_WELCOME(client) (std::string("001 ") + client + " :Welcome to the Internet Relay Network," + client + "!")
#define RPL_YOURHOST(client) (std::string("002 ") + client + " :Your host is localhost, running version 1.0")
#define RPL_CREATED(client) (std::string("003 ") + client + " :This server was created today")
#define RPL_MYINFO(client) (std::string("004 ") + client + " localhost 1.0 itkol")
#define ERR_NOTREGISTERED(client) (std::string("451 ") + client + " :You have not registered")

// ping pong
#define PING(msg) (std::string("PING :") + msg)
#define PONG(msg) (std::string("PONG :") + msg) //"PONG :"" ????

// kick
#define KICK_MSG(clientNick, clientUser, clientHost, channel, target, kickMsg) (std::string(":") + clientNick + "!" + clientUser + "@" + clientHost + " KICK " + channel + " " + target + " " + kickMsg)
#define ERR_CHANOPRIVSNEEDED(client, channel) (std::string("482 localhost ") + client + " " + channel + " :You're not channel operator")
#define ERR_NOTONCHANNEL(client, channel) (std::string("442 ") + client + " " + channel + " :You're not on that channel")
#define ERR_USERNOTINCHANNEL(client, nick, channel) (std::string("441 ") + client + " " + nick + " " + channel + " :They aren't on that channel")

// topic
#define RPL_NOTOPIC(client, channel) (std::string("331 ") + client + " " + channel + " No topic is set")
#define RPL_TOPIC(client, channel, topic) (std::string("332 ") + client + " " + channel + " " + topic)

// invite
#define ERR_USERONCHANNEL(client, nick, channel) (std::string("443 ") + client + " " + nick + " " + channel + " :is already on channel")
#define RPL_INVITING(client, nick, channel) (std::string("341 ") + client + " " + nick + " " + channel)
#define ERR_INVITEONLYCHAN(client, channel) (std::string("473 ") + client + " " + channel + " Cannot join channel (+i)")

// mode
#define RPL_CHANNELMODEIS(client, channel, modestring) (std::string("324 ") + client + " " + channel + " " + modestring)
#define ERR_INVALIDKEY(client, channel) (std::string("525 ") + client + " " + channel + " Key its not well formed")

#endif