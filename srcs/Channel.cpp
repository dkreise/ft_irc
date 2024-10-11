#include "../inc/Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string name) : _name(name), _topic(""), _client_limit(CLIENT_LIMIT)
{
    this->_modes['i'] = false;
    this->_modes['t'] = true;
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

void Channel::setClientLimit(int limit)
{
    this->_client_limit = limit;
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

bool Channel::getMode(char mode)
{
    bool status = this->_modes[mode];
    return (status);
}

int Channel::getClientLim(void) const
{
    return (this->_client_limit);
}

int Channel::getClientCnt(void) const
{
    return (this->_clients.size());
}

std::vector<int> Channel::getClients(void) const
{
    return (this->_clients);
}

void Channel::addClient(int& fd)
{
    this->_clients.push_back(fd);
}

bool Channel::isClientInChannel(int clntfd)
{
    std::vector<int>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == clntfd)
			return true;
	}
	return false;
}

bool Channel::isOperator(int clntfd)
{
    std::vector<int>::iterator it;

	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == clntfd)
			return true;
	}
	return false;
}

void Channel::addOperator(int clntfd)
{
    _operators.push_back(clntfd);
}

void Channel::removeOperator(int clntfd)
{
    std::vector<int>::iterator it;

	for (it = _operators.begin(); it != _operators.end(); it++)
	{
		if (*it == clntfd)
		{
            _operators.erase(it);
            return ;
        }	
	}
}

void Channel::removeClient(int& clntsock)
{
    std::vector<int>::iterator it;

	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		if (*it == clntsock)
		{
            _clients.erase(it);
            return ;
        }	
	}
}

void Channel::removeInvited(int& clntsock)
{
    std::vector<int>::iterator it;

	for (it = _invited.begin(); it != _invited.end(); it++)
	{
		if (*it == clntsock)
		{
            _invited.erase(it);
            return ;
        }	
	}
}

void Channel::addInvited(int& clntsock)
{
     _invited.push_back(clntsock);
}

bool Channel::isInvited(int& cltsock)
{
    for (size_t i = 0; i < _invited.size(); i++)
    {
        if (_invited[i] == cltsock)
            return true;
    }
    return false;
}

std::string Channel::getmodeString()
{
    std::string modestr = "+";

    if (_modes['i'])
        modestr += "i";
    if (_modes['t'])
        modestr += "t";
    if (_modes['l'])
        modestr += "l";
    if (_modes['k'])
        modestr += "k";

    if (modestr.size() == 1)
        return "";
    return modestr;
}

bool Channel::isEmpty(void)
{
    return this->_clients.empty();
}