#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

/* name of the channel:
- beginning with specified prefix characters:
regular channel, prefix character for this type of channel is ('#', 0x23)
local channel, prefix character for this type of channel is ('&', 0x26)
- may not contain any spaces (' ', 0x20), a control G / BELL ('^G', 0x07), 
or a comma (',', 0x2C) (which is used as a list item separator by the protocol)
*/

class Client;

class Channel
{
    public:
        Channel(void);
        Channel(std::string name);

        void setName(std::string name);
        void setTopic(std::string topic);
        void setKey(std::string key);
        void setMode(char mode, bool status); // to use only after checking that mode exists!
        void setClientLimit(int limit);

        std::string getName(void) const;
        std::string getTopic(void) const;
        std::string getKey(void) const;
        bool getMode(char mode); // to use only after checking that mode exists!
        int getClientLim(void) const;
        int getClientCnt(void) const;
        std::vector<int> getClients(void) const;

        void addClient(int& fd);
        //void sendMessage(const std::string& message) const;

        bool isClientInChannel(int clntfd);
    
        bool isOperator(int clntfd);
        void addOperator(int clntfd);
        void removeOperator(int clntfd);

        void removeClient(int& cltsock);

        void removeInvited(int& cltsock);
        void addInvited(int& cltsock);

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