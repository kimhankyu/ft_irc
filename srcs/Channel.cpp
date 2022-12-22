#include "Channel.hpp"
#include <iostream>
#include <algorithm>

Channel::Channel() : _mode(0) { }

Channel::Channel(std::string name, std::string key, User oper)
: _name(name), _key(key)
{
	_operators.push_back(oper);
	_userlist.push_back(oper);
}

int Channel::get_user_num() { return _userlist.size(); }
std::string Channel::get_key() const { return _key; }
std::string Channel::get_topic() const { return _topic; }
std::string Channel::get_mode() const
{
	std::string ret = "";
	int i = 1;
	while (i <= 4) {
		if (_mode & i) {
			if (i == 1) { ret += "i"; }
			else if (i == 2) { ret += "t"; }
			else if (i == 4) { ret += "k"; }
		}
		i <<= 1;
	}
	return "+" + ret;
}



void Channel::set_topic(const std::string str) { _topic = str; }
void Channel::set_mode(int mode, int flag, std::string arg)
{
	if (flag == 0) {
		_mode |= mode;
	} else if (flag == 1) {
		_mode &= !mode;
	}
	if (mode == KEY_CHANNEL_MODE) {
		_key = arg;
	}
}

bool Channel::is_user(User u)
{
	return std::find(_userlist.begin(), _userlist.end(), u) != _userlist.end();
}

bool Channel::is_operator(User u)
{
	return std::find(_operators.begin(), _operators.end(), u) != _operators.end();
}

bool Channel::is_mode(int mode)
{
	return _mode & mode;
}

void Channel::del_user(User u)
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

void Channel::add_user(const User u)
{
	if (_operators.size() == 0) {
		_operators.push_back(u);
	}
	_userlist.push_back(u);
}






void Channel::send_msg(const std::string str)
{
	std::vector<User>::iterator it = _userlist.begin();

	for (; it != _userlist.end(); ++it) {
		(*it).send_msg(str);
	}
}


Channel::~Channel() { }
