#include "Channel.hpp"
#include <iostream>
#include <algorithm>

Channel::Channel(std::string name, std::string key, User oper)
: _name(name), _key(key)
{
	
}

Channel::Channel() { }

Channel::~Channel() { }

int Channel::get_user_num() { return _userlist.size(); }

bool Channel::is_user(const User u)
{
	return std::find(_userlist.begin(), _userlist.end(), u) != _userlist.end();
}

bool Channel::is_operator(const User u)
{
	return std::find(_operators.begin(), _operators.end(), u) != _operators.end();
}

void Channel::cmd_part(User u)
{
	std::vector<User>::iterator it = std::find(_userlist.begin(), _userlist.end(), u);
	_userlist.erase(it);
	it = std::find(_operators.begin(), _operators.end(), u);
	if (it != _operators.end()) {
		_operators.erase(it);
		if (!_userlist.empty()) {
			_operators.push_back(_userlist[0]);
		}
	}
}



void Channel::send_msg(const std::string str)
{
	std::vector<User>::iterator it = _userlist.begin();

	for (; it != _userlist.end(); ++it) {
		(*it).send_msg(str);
	}
}


