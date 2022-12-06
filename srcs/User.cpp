#include "User.hpp"
#include <unistd.h>

User::User() : _fd(0)
{
	_is_registered = false;
	_is_authenticated = false;
	_is_admin = false;
	_nickname = "User" + std::to_string(_fd);
}


User::User(int fd) : _fd(fd)
{
	_is_registered = false;
	_is_authenticated = false;
	_is_admin = false;
	_nickname = "User" + std::to_string(_fd);
}

User& User::operator=(const User &user)
{
	_fd = user._fd;
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
	_is_registered = user._is_registered;
	_is_authenticated = user._is_authenticated;
	_is_admin = user._is_admin;
	_nickname = user._nickname;
	_username = user._username;
	_realname = user._realname;
}


std::string User::get_nickname() const { return _nickname; }

bool User::get_registered() const { return _is_registered; }


void User::send_msg(std::string msg)
{
	write(_fd, msg.c_str(), msg.size());
}

// void	User::send_err(std::string msg, int flag)
// {
// 	send(_fd, msg.c_str(), msg.size(), flag);
// 	throw std::runtime_error(msg.c_str());
// }



User::~User() { }
