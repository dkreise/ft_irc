#include "../../inc/Server.hpp"

void Server::_pass(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    std::string nick = this->_clients[sock].getNickname();

    if (this->_clients[sock].isAllowed() || this->_clients[sock].isRegistered())
    {
        this->_clients[sock].sendMessage(ERR_ALREADYREGISTERED(nick));
        return;
    }
    else if (args.size() < 2)
    {
        this->_clients[sock].sendMessage(ERR_NEEDMOREPARAMS(nick, "PASS"));
        return;
    }
    else if (args[1] == this->_password)
    {
        this->_clients[sock].setAllowedStatus(true);
        this->_clients[sock].setBuffer(""); // is not neccessary there..?
#ifdef DEBUG
        printf("Password match\n");
#endif
    }
    else
    {
        this->_clients[sock].sendMessage(ERR_PASSWDMISMATCH(nick));
        close(sock);
        this->_fds.erase(this->_fds.begin() + i);
        this->_nfds--;
        i--;
    }
}