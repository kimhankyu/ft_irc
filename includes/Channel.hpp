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
	std::string	_type;
	std::string	_topic;
	std::vector<User> operators;
	std::vector<User> userlist;

	Channel();

public:
	// JOIN <channel> <key> 검증
	void join(User newuser);

	// channel 객체 생성 전에 name validity 검증해야 함
	Channel(std::string name, std::string key, User oper);
	~Channel();
};

#endif