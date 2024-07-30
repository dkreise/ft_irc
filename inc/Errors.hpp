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


#endif