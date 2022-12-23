#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"

#define INVITE_ONLY_CHANNEL_MODE	1 // i
#define PROTECTED_TOPIC_MODE		2 // t
#define KEY_CHANNEL_MODE			4 // k

class Channel
{
private:
	int			_mode;
	std::string	_name;
	std::string _key;
	std::string	_topic;
	std::vector<User> _operators;
	std::vector<User> _userlist;

//TODO - operator=
public:
	Channel();
	Channel(std::string name, User oper);
	~Channel();

	int get_user_num();
	std::string get_key() const;
	std::string get_topic() const;
	std::string get_mode() const;
	
	void set_topic(const std::string str);
	void set_mode(int mode, int flag, std::string arg = "");


	bool is_user(User u);
	bool is_operator(User u);
	bool is_mode(int mode);
	bool is_key_same(const std::string str);
	void del_user(User u);
	void add_user(const User u);

	void send_msg(const std::string str);
};

#endif