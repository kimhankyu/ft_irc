#include "Channel.hpp"
#include <iostream>
#include <algorithm>

void	Channel::join(User newuser)
{
	if (std::find(userlist.begin(), userlist.end(), newuser) != userlist.end())
	{
		
	}
}

Channel::Channel(std::string name, std::string key, User oper)
: _name(name), _key(key)
{
	
}

Channel::Channel() { }

Channel::~Channel() { }
