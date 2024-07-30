#ifndef ERRORS_HPP
#define ERRORS_HPP

// pass
#define ERR_PASSWDMISMATCH (std::string("464") + " :Password incorrect\n")
#define ERR_NEEDMOREPARAMS (std::string("461") + " :Not enough parameters\n")
#define ERR_ALREADYREGISTERED (std::string("462") + " :You may not reregister\n")

// nick
#define ERR_NONICKNAMEGIVEN (std::string("431") + " :No nickname is given\n")
#define ERR_ERRONEUSNICKNAME (std::string("432") + " :Invalid nickname\n")
#define ERR_NICKNAMEINUSE (std::string("433") + " :Nickname is in use\n")

// join
#define ERR_NOSUCHCHANNEL (std::string("403") + " :No such channel exists\n")
#define ERR_TOOMANYCHANNELS (std::string("405") + " :Too many channels\n")
#define ERR_BADCHANNELKEY (std::string("475") + " :Bad channel key\n")
#define ERR_BANNEDFROMCHAN (std::string("474") + " :Banned from channel\n")
#define ERR_CHANNELISFULL (std::string("471") + " :Channel is full\n")
#define ERR_INVITEONLYCHAN (std::string("473") + " :Channel is invite-only\n")
#define ERR_BADCHANMASK (std::string("476") + " :Bad Channel Mask\n")

#endif