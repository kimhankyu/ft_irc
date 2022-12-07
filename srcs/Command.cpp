#include "Server.hpp"
#include "User.hpp"
#include "Reply.hpp"
#include "Util.hpp"

#include <iostream>

void Server::cmd_pass(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 2) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
	} else if (_users[fd].get_registered()) {
		_users[fd].send_msg(ERR_ALREADYREGISTRED(_users[fd].get_nickname()));
	} else if (_password != v[1]) {
		_users[fd].send_msg(ERR_PASSWDMISMATCH(_users[fd].get_nickname()));
	}
}

/*
클라이언트 꺼지면 닉네임 삭제
*/

static bool validateNick(std::string str)
{
	if (str.length() > 9)
		return false;
	if (!(isLetter(str[0]) || isSpecial(str[0])))
		return false;
	for (size_t i = 1; i < str.length(); ++i) {
		if (!(isLetter(str[i]) || isNumber(str[i])
		|| isSpecial(str[i]) || str[i] == '-')) {
			return false;
		}
	}
	return true;
}

bool Server::find_nickname(std::string str)
{
	std::map<int, std::string>::iterator it = _nicknames.begin();

	for (; it != _nicknames.end(); ++it) {
		if (it->second == str) {
			return false;
		}
	}
	return true;
}


void Server::cmd_nick(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 2) { //ERR_NONICKNAMEGIVEN
		_users[fd].send_msg(ERR_NONICKNAMEGIVEN);
	} else if (!validateNick(v[1])) { // 유효하지 않은 닉네임 ERR_ERRONEUSNICKNAME
		_users[fd].send_msg(ERR_ERRONEUSNICKNAME(_users[fd].get_nickname(), v[1]));
	} else if (!find_nickname(v[1])) { // 이미 존재하는 닉네임 ERR_NICKNAMEINUSE
		_users[fd].send_msg(ERR_NICKNAMEINUSE(_users[fd].get_nickname(), v[1]));
	} else {
		_users[fd].send_msg(RPY_NICK(_users[fd].get_nickname(), v[1]));
		_users[fd].set_nickname(v[1]);
		_nicknames[fd] = v[1];
	}
}

void Server::cmd_user(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 5) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
	} else if (_users[fd].get_registered()) {
		_users[fd].send_msg(ERR_ALREADYREGISTRED(_users[fd].get_nickname()));
	} else {
		_users[fd].set_username(v[1]);
		_users[fd].set_hostname(v[2]);
		_users[fd].set_servername(v[3]);
		std::size_t pos = v[4].find(":"); 
		std::string realname = v[4].substr(pos + 1);
		for (size_t i = 4; i < v.size(); ++i) {
			realname += " " + v[i];
		}
		_users[fd].set_realname(realname);
		_users[fd].set_registered(true);
		_users[fd].send_msg(RPY_WELCOME(_users[fd].get_nickname()));
	}
}

void Server::cmd_pong(std::vector<std::string> &v, const int fd)
{

}



