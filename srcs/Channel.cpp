#include "../inc/Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string name) : _name(name)
{
    this->_modes['i'] = false;
    this->_modes['t'] = false;
    this->_modes['k'] = false;
    this->_modes['o'] = false;
    this->_modes['l'] = false;
}

void Channel::setName(std::string name)
{
    this->_name = name;
}

void Channel::setTopic(std::string topic)
{
    this->_topic = topic;
}

void Channel::setKey(std::string key)
{
    this->_key = key;
}

void Channel::setMode(char mode, bool status)
{
    this->_modes[mode] = status;
}

std::string Channel::getName(void) const
{
    return (this->_name);
}

std::string Channel::getTopic(void) const
{
    return (this->_topic);
}

std::string Channel::getKey(void) const
{
    return (this->_key);
}

bool Channel::getMode(char mode) const
{
    return (this->_modes[mode]);
}

void Channel::addClient(int fd)
{
    this->_clients.push_back(fd);
}