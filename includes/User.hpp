#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>
#include <set>

#define INVISIBLE_USER_MODE		1 //i
#define OPER_USER_MODE			2 //o
#define LOCAL_OPER_USER_MODE	4 //O
#define REGISTERED_USER_MODE	8 //r
#define WALLOPS_USER_MODE		16 //w

class User
{
private:
	int			_fd;
	int			_mode;
	bool		_is_registered;
	bool		_is_authenticated;
	bool		_is_admin;
	std::string	_nickname;
	std::string	_username;
	std::string	_hostname;
	std::string	_servername;
	std::string	_realname;
	std::set<std::string> _channels;

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
	bool get_admin() const;
	std::string get_mode() const;
	
	int get_channels_num() const;

	void set_nickname(std::string str);
	void set_username(std::string str);
	void set_hostname(std::string str);
	void set_servername(std::string str);
	void set_realname(std::string str);
	void set_registered(bool b);
	void set_admin(bool b);
	void set_mode(int mode, int flag);

	// bool is_channel_user(const Channel c);
	void add_channel(const std::string c);
	void remove_channel(const std::string c);

	void send_msg(const std::string msg);


	bool operator==(const User &u);
};

#endif
