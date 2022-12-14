#include "User.hpp"
#include <unistd.h>

User::User() : _fd(0), _mode(0)
{
	_is_registered = false;
	_is_authenticated = false;
	_is_admin = false;
	_nickname = "User" + std::to_string(_fd);
}


User::User(int fd) : _fd(fd), _mode(0)
{
	_is_registered = false;
	_is_authenticated = false;
	_is_admin = false;
	_nickname = "User" + std::to_string(_fd);
}

User& User::operator=(const User &user)
{
	_fd = user._fd;
	_mode = user._mode;
	_is_registered = user._is_registered;
	_is_authenticated = user._is_authenticated;
	_is_admin = user._is_admin;
	_nickname = user._nickname;
	_username = user._username;
	_realname = user._realname;
	return (*this);
}

User::User(const User& user)
{
	_fd = user._fd;
	_mode = user._mode;
	_is_registered = user._is_registered;
	_is_authenticated = user._is_authenticated;
	_is_admin = user._is_admin;
	_nickname = user._nickname;
	_username = user._username;
	_realname = user._realname;
}


std::string User::get_nickname() const { return _nickname; }
std::string User::get_username() const { return _username; }
std::string User::get_hostname() const { return _hostname; }
std::string User::get_servername() const { return _servername; }
std::string User::get_realname() const { return _realname; }
bool User::get_registered() const { return _is_registered; }
bool User::get_admin() const { return _is_admin; }

std::string User::get_mode() const {
	std::string ret = "";
	int i = 1;
	while (i <= 16) {
		if (_mode & i) {
			if (i == 1) { ret += "i"; }
			else if (i == 2) { ret += "o"; }
			else if (i == 4) { ret += "O"; }
			else if (i == 8) { ret += "r"; }
			else if (i == 16) { ret += "w"; }
		}
		i <<= 1;
	}
	return ret;
}

void User::set_nickname(std::string str) { _nickname = str; }
void User::set_username(std::string str) { _username = str; }
void User::set_hostname(std::string str) { _hostname = str; }
void User::set_servername(std::string str) { _servername = str; }
void User::set_realname(std::string str) { _realname = str; }
void User::set_registered(bool b) { _is_registered = b; }
void User::set_admin(bool b) { _is_admin = b; }

void User::set_mode(int mode, int flag) {
	if (flag == 0) {
		_mode |= mode;
		if (mode == OPER_USER_MODE) {
			_is_admin = true;
		}
	} else if (flag == 1) {
		_mode &= !mode;
		if (mode == OPER_USER_MODE) {
			_is_admin = false;
		}
	}
}

void User::send_msg(std::string msg)
{
	write(_fd, msg.c_str(), msg.size());
}



User::~User() { }
