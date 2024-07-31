#ifndef ERRORS_HPP
#define ERRORS_HPP

// pass
#define ERR_PASSWDMISMATCH(client) (std::string("464 ") + client + " :Password incorrect")
#define ERR_NEEDMOREPARAMS(client, cmd) (std::string("461 ") + client + " " + cmd + " :Not enough parameters")
#define ERR_ALREADYREGISTERED(client) (std::string("462 ") + client + " :You may not reregister")

// nick
#define ERR_NONICKNAMEGIVEN(client) (std::string("431 ") + client + " :No nickname given")
#define ERR_ERRONEUSNICKNAME(client, nick) (std::string("432 ") + client + " " + nick + " :Erroneus nickname")
#define ERR_NICKNAMEINUSE(client, nick) (std::string("433 ") + client + " " + nick + " :Nickname is already in use")

// join
#define ERR_NOSUCHCHANNEL(client, channel) (std::string("403 ") + client + " " + channel + " :No such channel exists")
#define ERR_TOOMANYCHANNELS(client, channel) (std::string("405 ") + client + " " + channel + " :You have joined too many channels")
#define ERR_BADCHANNELKEY(client, channel) (std::string("475 ") + client + " " + channel + " :Cannot join channel (+k)")
#define ERR_BANNEDFROMCHAN(client, channel) (std::string("474 ") + client + " " + channel + " :Cannot join channel (+b)")
#define ERR_CHANNELISFULL(client, channel) (std::string("471 ") + client + " " + channel + " :Cannot join channel (+l)")
#define ERR_INVITEONLYCHAN(client, channel) (std::string("473 ") + client + " " + channel + " :Cannot join channel (+i)")
#define ERR_BADCHANMASK(channel) (std::string("476 ") + channel + " :Bad Channel Mask")

#endif