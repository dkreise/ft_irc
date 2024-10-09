#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"

#define MAX_CHANNEL 5

class Client
{
    public:
        Client(void) {}
        Client(int sock);
        ~Client();

        void setSock(int& sock);
        void setNickname(std::string& nickname);
        void setUsername(std::string& username);
        void setHostname(std::string& hostname);
        void setRealname(std::string& realname);
        void setBuffer(std::string buffer);
        void addBuffer(std::string& buffer);
        void setAllowedStatus(bool isAllowed);
        void setRegisteredStatus(bool isRegistered);
        void addChannel(std::string& channel, bool admin);

        int getSock(void) const;
        std::string getNickname(void) const;
        std::string getUsername(void) const;
        std::string getHostname(void) const;
        std::string getRealname(void) const;
        std::string getBuffer(void) const;
        bool isAllowed(void) const;
        bool isRegistered(void) const;
        int getChannelCnt(void) const;
        int getChannelLim(void) const;
        bool isAdmin(std::string& channel);

        void sendMessage(const std::string& message) const;

        void removeFromChan(std::string& channelname); 
        
    private:
        int _sock;
        std::string _nickname;
        std::string _username;
        std::string _hostname;
        std::string _realname;
        std::string _buffer;
        bool _isAllowed; // if has passed pass command properly
        bool _isRegistered; // if it has used nick and/or (??) name command
        //std::vector<std::string> _channels;
        std::map<std::string, bool> _channels;
        int _chan_limit;

};

#endif