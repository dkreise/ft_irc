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
        void addChannel(std::string& channel, bool admin);

        int getSock(void) const;
        std::string getNickname(void) const;
        std::string getUsername(void) const;
        std::string getBuffer(void) const;
        bool isAllowed(void) const;
        bool isRegistered(void) const;
        int getChannelCnt(void) const;
        int getChannelLim(void) const;
        bool isAdmin(std::string& channel);

        void sendMessage(const std::string& message) const;
        
    private:
        int _sock;
        std::string _nickname;
        std::string _username;
        std::string _buffer;
        bool _isAllowed;
        bool _isRegistered;
        //std::vector<std::string> _channels;
        std::map<std::string, bool> _channels;
        int _chan_limit;
};

#endif