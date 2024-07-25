#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>


class Server
{
    public:
        Server(void);
        Server(int port);

        void start(void);


    private:
        int _port;
        int _sock;
};

#endif