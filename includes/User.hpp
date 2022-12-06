#ifndef USER_HPP
#define USER_HPP

#include <string>

class User
{
private:
	int			_fd;
	bool		_is_registered;
	bool		_is_authenticated;
	bool		_is_admin;
	std::string	_nickname;
	std::string	_username;
	std::string	_realname;

public:
	User();
	User(int fd);
	User& operator=(const User &user);
	User(const User& user);
	~User();
	std::string get_nickname() const;
	bool get_registered() const;
	void send_msg(std::string msg);

};

#endif
