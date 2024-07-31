#include "../inc/Client.hpp"

Client::Client(int sock) : _sock(sock), _isAllowed(false), _isRegistered(false) {}

Client::~Client() {}

void Client::setSock(int& sock)
{
    this->_sock = sock;
}

void Client::setNickname(std::string& nickname)
{
    this->_nickname = nickname;
}

void Client::setUsername(std::string& username)
{
    this->_username = username;
}

void Client::setBuffer(std::string buffer)
{
    this->_buffer = buffer;
}

void Client::addBuffer(std::string& buffer)
{
    this->_buffer += buffer;
}

void Client::setAllowedStatus(bool isAllowed)
{
    this->_isAllowed = isAllowed;
}

void Client::setRegisteredStatus(bool isRegistered)
{
    this->_isRegistered = isRegistered;
}

void Client::addChannel(std::string& channel)
{
    this->_channels.push_back(channel);
}

int Client::getSock(void) const
{
    return (this->_sock);
}

std::string Client::getNickname(void) const
{
    return (this->_nickname);
}

std::string Client::getUsername(void) const
{
    return (this->_username);
}

std::string Client::getBuffer(void) const
{
    return (this->_buffer);
}

bool Client::isAllowed(void) const
{
    return (this->_isAllowed);
}

bool Client::isRegistered(void) const
{
    return (this->_isRegistered);
}

void Client::sendMessage(const std::string& message) const
{
    std::string msg = message + "\r\n";
    send(this->_sock, msg.c_str(), msg.length(), 0);
}








// int main(void)
// {
//     struct pollfd fds[2];

//     int sock_cl = socket(AF_INET, SOCK_STREAM, 0); 
//     struct sockaddr_in info;
//     info.sin_family = AF_INET;
//     info.sin_port = htons(8081);
//     info.sin_addr.s_addr = INADDR_ANY;

//     connect(sock_cl, (struct sockaddr*) &info, sizeof(info));

//     fds[0].fd = sock_cl;
//     fds[0].events = POLLIN;
//     fds[1].fd = STDIN_FILENO;
//     fds[1].events = POLLIN;

//     char buffer[1024] = {0};
//     while (1)
//     {
//         int ret = poll(fds, 2, -1);
//         if (ret == -1)
//         {
//             std::cerr << "Error poll" << std::endl;
//             return 1;
//         }
//         if (fds[0].revents == POLLIN)
//         {
//             memset(buffer, 0, 1024);
//             ssize_t bytes_read = read(sock_cl, buffer, 1024);
//             if (bytes_read > 0) {
//                 printf("from other client: %.*s", (int)bytes_read, buffer);
//             }
//         }
//         if (fds[1].revents == POLLIN)
//         {
//             memset(buffer, 0, 1024);
//             if (fgets(buffer, 1024, stdin) != NULL)
//                 write(sock_cl, buffer, strlen(buffer));
//         }
//     }
// }