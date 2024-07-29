#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

class Client
{
    public:
        Client(void) {}
        Client(int sock);
        ~Client();

        void setSock(int& sock);
        void setNickname(std::string& nickname);
        void setUsername(std::string& username);
        void setBuffer(std::string buffer);
        void addBuffer(std::string& buffer);
        void setAllowedStatus(bool isAllowed);
        void setRegisteredStatus(bool isRegistered);

        int getSock(void) const;
        std::string getNickname(void) const;
        std::string getUsername(void) const;
        std::string getBuffer(void) const;
        bool isAllowed(void) const;
        bool isRegistered(void) const;
        
    private:
        int _sock;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
        bool _isAllowed;
        bool _isRegistered;
};

#endif