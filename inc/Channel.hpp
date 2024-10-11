#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

class Client;

class Channel
{
    public:
        Channel(void);
        Channel(std::string name);

        void setName(std::string name);
        void setTopic(std::string topic);
        void setKey(std::string key);
        void setMode(char mode, bool status);
        void setClientLimit(int limit);

        std::string getName(void) const;
        std::string getTopic(void) const;
        std::string getKey(void) const;
        bool getMode(char mode);
        int getClientLim(void) const;
        int getClientCnt(void) const;
        std::vector<int> getClients(void) const;

        void addClient(int& fd);
        void removeClient(int& cltsock);

        bool isClientInChannel(int clntfd);
    
        bool isOperator(int clntfd);
        void addOperator(int clntfd);
        void removeOperator(int clntfd);

        void removeInvited(int& cltsock);
        void addInvited(int& cltsock);
        bool isInvited(int& cltsock);
        bool isEmpty(void);

        std::string getmodeString();

    private:
        std::string _name;
        std::string _topic;
        // std::pair
        std::string _key;
        std::map<char, bool> _modes;
        std::vector<int> _clients; // fds of clients
        int _client_limit;

        std::vector<int> _operators;// fds of clients operators
        std::vector<int> _invited; // fds of invited clients

};

#endif