#include "Channel.hpp"
#include <iostream>
#include <algorithm>

Channel::Channel(std::string name, std::string key, User oper)
: _name(name), _key(key)
{
	
}

Channel::Channel() { }

Channel::~Channel() { }
