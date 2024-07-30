#include "../inc/Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string name) : _name(name) {}

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

void Channel::addClient(int fd)
{
    this->_clients.push_back(fd);
}