#include "Server.hpp"

void Server::_pass(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;

    if (this->_clients[sock].isAllowed() || this->_clients[sock].isRegistered())
    {
        std::cout << ERR_ALREADYREGISTERED;
        return; // send error alreadyregistered
    }
    else if (args.size() != 2)
    {
        std::cout << ERR_NEEDMOREPARAMS;
        return; // send error needmoreparams
    }
    else if (args[1] == this->_password)
    {
        this->_clients[sock].setAllowedStatus(true);
        this->_clients[sock].setBuffer("");
        printf("Password match\n");
    }
    else
    {
        close(sock);
        this->_fds.erase(this->_fds.begin() + i);
        this->_nfds--;
        i--;
        // send error passwordmismatch
        std::cout << ERR_PASSWDMISMATCH;
    }
}