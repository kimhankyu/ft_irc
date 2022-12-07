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
std::string User::get_username() const { return _username; }
std::string User::get_hostname() const { return _hostname; }
std::string User::get_servername() const { return _servername; }
std::string User::get_realname() const { return _realname; }
bool User::get_registered() const { return _is_registered; }

void User::set_nickname(std::string str) { _nickname = str; }
void User::set_username(std::string str) { _username = str; }
void User::set_hostname(std::string str) { _hostname = str; }
void User::set_servername(std::string str) { _servername = str; }
void User::set_realname(std::string str) { _realname = str; }
void User::set_registered(bool b) { _is_registered = b; }



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
