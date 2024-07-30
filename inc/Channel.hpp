#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"

class Channel
{
    public:
        Channel(void);
        Channel(std::string name);

        void setName(std::string name);
        void setTopic(std::string topic);
        void setKey(std::string key);

        std::string getName(void) const;
        std::string getTopic(void) const;
        std::string getKey(void) const;

        void addClient(int fd);
    
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        std::vector<int> _clients; // fds of clients
};

#endif