#include "../inc/Server.hpp"

bool g_signal = false;

Server::Server(int port, std::string password) : _port(port), _password(password), _nfds(1)
{
    this->_sock = socket(AF_INET, SOCK_STREAM, 0); // protect
    
    this->_info.sin_family = AF_INET;
    this->_info.sin_port = htons(port);
    this->_info.sin_addr.s_addr = INADDR_ANY;
    int opt = 1;

    // setting options for socket
    if (setsockopt(this->_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // setting socket to non-blocking
    if (fcntl(this->_sock, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Error setting socket to non-blocking" << std::endl;
        close(this->_sock);
        //return 1; // throw exception instead ?
    }

    // connecting socket to port
    if (bind(this->_sock, (struct sockaddr*) &(this->_info), sizeof(this->_info)) < 0)
    {
        std::cerr << "Error binding" << std::endl;
        //return (1); // throw exception instead ?
    }

    listen(this->_sock, SOMAXCONN);
    struct pollfd pfd;
    this->_fds.push_back(pfd);
    this->_fds[0].fd = this->_sock;
    this->_fds[0].events = POLLIN;
    this->_fds[0].revents = 0;

}

void Server::doPollLoop(void)
{
    signal(SIGINT, Server::_signalHandler);
    while (g_signal == false)
    {
        int pollCount = poll(this->_fds.data(), this->_nfds, -1);
        if (pollCount == -1 && g_signal == false) // set timeout !!
        {
            std::cerr << "Error poll" << std::endl;
            continue ;
        }

        for (int i = 0; i < this->_nfds; i ++)
        {
            if (this->_fds[i].revents == 0)        
                continue;
            if (this->_fds[i].fd == this->_sock)
            {
                acceptNewClient();
            }
            else if (this->_fds[i].revents == POLLIN)
            {
                receiveMessage(i);
            }
        }
    }
    _closeAndClean();
}

void Server::acceptNewClient(void)
{
    int sock_cl = accept(this->_sock, NULL, NULL); // protect ?
    Client client(sock_cl);
    client.setSock(sock_cl);
#ifdef DEBUG
    printf("New client connected\n");
#endif
    this->_clients[sock_cl] = client;
    struct pollfd pfd;
    this->_fds.push_back(pfd);
    this->_fds[this->_nfds].fd = sock_cl;
    this->_fds[this->_nfds].events = POLLIN;
    this->_fds[this->_nfds].revents = 0;
    this->_nfds ++;
}

void Server::receiveMessage(int& i)
{
    char buffer[1024] = {0};
    memset(buffer, 0, 1024);
    size_t bytes_read = read(this->_fds[i].fd, buffer, 1024);

    if (bytes_read <= 0)
    {
        close(this->_fds[i].fd);
        this->_fds.erase(this->_fds.begin() + i);
        this->_nfds  --;
        i--;
    }
    else
    {
        std::string buf(buffer, bytes_read);
    
        this->_clients[this->_fds[i].fd].addBuffer(buf);
        buf = this->_clients[this->_fds[i].fd].getBuffer();
        std::vector<std::string> msgs = _parseBuffer(buf);
        this->_clients[this->_fds[i].fd].setBuffer("");

        int msg_cnt = msgs.size();
        int i_cur = i;

        for (int m = 0; m < msg_cnt; m ++)
        {
            if (i_cur != i) // for checking if the client is still alive
                return;
            if (m == msg_cnt - 1 && (buf[buf.size() - 1] != '\n' || buf[buf.size() - 2] != '\r'))
            {
                this->_clients[this->_fds[i].fd].setBuffer(msgs[m]);
            }
            else
            {
                checkMessage(i, msgs[m]);
            }
        }
    }
}

std::vector<std::string> Server::_parseBuffer(std::string& str)
{
    std::vector<std::string> msgs;
    size_t start = 0;
    size_t pos = 0;

#ifdef DEBUG
    //printf("Parsing buffer\n");
#endif

    while ((pos = str.find("\r\n", start)) != std::string::npos)
    {
        msgs.push_back(str.substr(start, pos - start));
        start = pos + 2;
    }
    if (start < str.size())
    {
        msgs.push_back(str.substr(start));
    }
    return (msgs);
}

void Server::checkMessage(int& i, std::string& msg)
{
    std::vector<std::string> args;
    int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];

    std::cout << "->" << msg << "<-" << std::endl;

    args = _parseMessage(msg, ' ');
    if (args.size() == 0)
        return;

    std::string cmds[10] = {"PASS", "NICK", "USER", "PING", "PRIVMSG", "JOIN", "TOPIC", "KICK", "INVITE", "MODE"};
	void (Server::*f[10])(int &client_fd, std::vector<std::string> &args) = {&Server::_pass, &Server::_nick, &Server::_user, &Server::_ping, &Server::_privmsg, &Server::_join, &Server::_topic, &Server::_kick, &Server::_invite, &Server::_mode};

    for (int j = 0; j < 10; j++)
    {
        if (j > 0 && !client.isAllowed())
        {
            //client.sendMessage(ERR_PASSWDMISMATCH(""));
            close(sock);
            this->_clients.erase(sock);
            this->_fds.erase(this->_fds.begin() + i);
            this->_nfds--;
            i--;
            return;
        }
        if (args[0] == cmds[j])
        {
            if ((j > 0 && !client.isAllowed()) || (j > 2 && !client.isRegistered()))
            {
                client.sendMessage("we are here");
                client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
                return;
            }
            (this->*f[j])(i, args);
            return;
        }
    }

    // if (args[0] == "PASS")
    // {
    //     _pass(i, args);
    // }
    // else if (args[0] == "NICK")
    // {
    //     _nick(i, args);
    // }
    // else if (args[0] == "JOIN")
    // {
    //     _join(i, args);
    // }
    // else if (args[0] == "PRIVMSG")
    // {
    //     _privmsg(i, args);
    // }
    // else if (!client.isRegistered()) // change with join
    // {
    //     client.sendMessage(ERR_NOTREGISTERED(client.getNickname()));
    //     return;
    // }

}

std::vector<std::string> Server::_parseMessage(std::string& str, char delim)
{
    std::vector<std::string> args;

    //str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    //str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        args.push_back(token);
    }
    return args;
}

void Server::sendMessageToChannel(int cl, Channel& chan, const std::string& message)
{
    int n = chan.getClientCnt();
    std::vector<int> socks = chan.getClients();

    for (int i = 0; i < n; i ++)
    {
        if (socks[i] != cl)
            this->_clients[socks[i]].sendMessage(message);
    }
}

void Server::_ping(int& i, std::vector<std::string>& args)
{
    int sock = this->_fds[i].fd;
    Client& client = this->_clients[sock];

    // std::cout << "Pong:" << args[1] << std::endl;
    client.sendMessage(PONG(args[1]));
}

void	Server::_closeAndClean()
{
	std::cout << "losing Server^" << std::endl;

	if (_sock != -1)
		close(_sock);
	
	std::map<int, Client>::iterator it = _clients.begin();
	while (it != _clients.end())
	{
		if (it->first != -1)
			close(it->first);
		it++;
	}
	
	_clients.clear();
	_fds.clear();
}

void	Server::_signalHandler(int signum)
{
	(void)signum;
	g_signal = true;
}

// void Server::_removeClntFromChan(int cltsock, std::string& channelname)
// {

// }



// int main()
// {
//     std::vector<struct pollfd> fds;
//     int nfds = 1;

//     int sock = socket(AF_INET, SOCK_STREAM, 0); 
//     struct sockaddr_in info;
//     info.sin_family = AF_INET;
//     info.sin_port = htons(8081);
//     info.sin_addr.s_addr = INADDR_ANY;
//     int opt = 1;

//     // Forcefully attaching socket to the port 8080
//     if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
//     {
//         perror("setsockopt");
//         exit(EXIT_FAILURE);
//     }

//     if (bind(sock, (struct sockaddr*) &info, sizeof(info)) < 0)
//     {
//         std::cerr << "Error binding" << std::endl;
//         return 1;
//     }
//     listen(sock, SOMAXCONN);

//     fds.push_back({sock, POLLIN});

//     char buffer[1024] = {0};
//     while (1)
//     {
//         int ret = poll(fds.data(), nfds, -1);
//         if (ret < 0)
//         {
//             std::cerr << "Error poll" << std::endl;
//             return 1;
//         }
//         int cur_size = nfds;
//         for (int i = 0; i < cur_size; i ++)
//         {
//             if (fds[i].revents == 0)
//                 continue;
//             if (fds[i].fd == sock)
//             {
//                 int client = accept(sock, NULL, NULL);
//                 fds.push_back({client, POLLIN});
//                 nfds ++;
//             }
//             else
//             {
//                 memset(buffer, 0, 1024);
//                 ssize_t bytes_read = read(fds[i].fd, buffer, 1024);
//                 if (bytes_read <= 0)
//                 {
//                     close(fds[i].fd);
//                     fds.erase(fds.begin() + i);
//                     nfds--;
//                     i--;
//                 }
//                 else
//                 {
//                     printf("from client%d: %s", i, buffer);
//                     for (int k = 0; buffer[k] != 0; k++)
//                     {
//                         printf("%d\n", buffer[k]);
//                     }
//                     for (int j = 1; j < nfds; j++)
//                     {
//                         if (j != i)
//                             write(fds[j].fd, buffer, strlen(buffer));
//                     }
//                 }
//             }
//         }
//     }
// }