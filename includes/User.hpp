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
	std::string	_hostname;
	std::string	_servername;
	std::string	_realname;

public:
	User();
	User(int fd);
	User& operator=(const User &user);
	User(const User& user);
	~User();
	std::string get_nickname() const;
	std::string get_username() const;
	std::string get_hostname() const;
	std::string get_servername() const;
	std::string get_realname() const;
	bool get_registered() const;

	void set_nickname(std::string str);
	void set_username(std::string str);
	void set_hostname(std::string str);
	void set_servername(std::string str);
	void set_realname(std::string str);
	void set_registered(bool b);

	void send_msg(std::string msg);

};

#endif
