#include "../../inc/Server.hpp"

void Server::_topic(int& i, std::vector<std::string>& args)
{
    // only for operators???

    int sock = this->_fds[i].fd;
    Client client = this->_clients[sock];
    std::string nick = client.getNickname();

    (void) args;
}