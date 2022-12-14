#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "User.hpp"

#define CHAN_LEN 200

class Channel
{
private:
	std::string	_name;
	std::string _key;
	std::string	_topic;
	std::vector<User> _operators;
	std::vector<User> _userlist;

	//u.push_back()
	//u[0] u[1][2][3][4]
	//u[0][1][2][3]

	Channel();

public:

	
	Channel(std::string name, std::string key, User oper);
	~Channel();
};

#endif