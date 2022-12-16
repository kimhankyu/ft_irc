#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"

class Channel
{
private:
	std::string	_name;
	std::string _key;
	std::string	_topic;
	std::vector<User> _operators;
	std::vector<User> _userlist;



	Channel();

public:
	Channel(std::string name, std::string key, User oper);
	~Channel();

	int get_user_num();
	std::string get_key() const;
	bool is_user(const User u);
	bool is_operator(const User u);
	void del_user(User u);
	void add_user(const User u);
	
	void send_msg(const std::string str);


};

#endif