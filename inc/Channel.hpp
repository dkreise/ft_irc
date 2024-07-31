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

class Channel
{
    public:
        Channel(void);
        Channel(std::string name);

        void setName(std::string name);
        void setTopic(std::string topic);
        void setKey(std::string key);
        void setMode(char mode, bool status); // to use only after checking that mode exists!

        std::string getName(void) const;
        std::string getTopic(void) const;
        std::string getKey(void) const;
        bool getMode(char mode) const; // to use only after checking that mode exists!

        void addClient(int fd);
    
    private:
        std::string _name;
        std::string _topic;
        std::string _key;
        std::map<char, bool> _modes;
        std::vector<int> _clients; // fds of clients
        int _client_limit;
};

#endif