#include "Server.hpp"
#include "User.hpp"
#include "Reply.hpp"
#include "Util.hpp"
#include <map>

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

std::map<int, std::string>::iterator Server::find_nickname(std::string str)
{
	std::map<int, std::string>::iterator it = _nicknames.begin();

	for (; it != _nicknames.end(); ++it) {
		if (it->second == str) {
			break;
		}
	}
	return it;
}

void Server::cmd_nick(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 2) {
		_users[fd].send_msg(ERR_NONICKNAMEGIVEN);
	} else if (!validateNick(v[1])) {
		_users[fd].send_msg(ERR_ERRONEUSNICKNAME(_users[fd].get_nickname(), v[1]));
	} else if (find_nickname(v[1]) != _nicknames.end()) {
		_users[fd].send_msg(ERR_NICKNAMEINUSE(_users[fd].get_nickname(), v[1]));
	} else {
		_users[fd].send_msg(RPL_NICK(_users[fd].get_nickname(), v[1]));
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
		_users[fd].set_realname(v[4]);
		_users[fd].set_registered(true);
		_users[fd].send_msg(RPL_WELCOME(_users[fd].get_nickname()));
	}
}

void Server::cmd_ping(std::vector<std::string> &v, const int fd)
{
	if (v.size() == 1) {
		_users[fd].send_msg(ERR_NOORIGIN);
	} else {
		//TODO - 시간 측정해서 시간 초과하면 quit 호출
		_users[fd].send_msg("PONG " + v[1] + "\n");
	}
}

void Server::cmd_oper(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 3) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
	} else {
		if (v[2] != OPER_PASS) {
			_users[fd].send_msg(ERR_PASSWDMISMATCH(_users[fd].get_nickname()));
		} else if (v[1] == OPER_NICK && v[2] == OPER_PASS) {
			_users[fd].send_msg(RPL_YOUREOPER(v[1]));
			_users[fd].set_mode(OPER_USER_MODE, 0);
			_users[fd].send_msg(":ircserv MODE " + v[1] + " " + "+o" + "\n");
		}
	}
}

/*
JOIN
PART

	TOPIC
	INVITE
	KICK
*/

void Server::cmd_join(std::vector<std::string> &v, const int fd)
{
	/*
	invite-only 인 경우에 사용자를 초대
	*/
	
}

void Server::cmd_part(std::vector<std::string> &v, const int fd)
{
	//v[0] == part
	//v[1] == channel,channel,
	//v[1]을 ','기준으로 스플릿
	//loop
	//1. 실제 있는 채널인지 확인
	//2. 해당 채널에서 유저가 존재하는지 확인
	//여러 가능성 고려하기 -> 1. 유저가 oper인 경우, 2. 해당 유저가 마지막이었던 경우
	std::vector<std::string> chanList = ft_split(v[1], ',');
	for (std::vector<std::string>::iterator it = chanList.begin(); it != chanList.end(); ++it)
	{
		if (_channels.find(*it) != _channels.end())
		{
			if (_channel[*it]._member)
		}
	}
}

void Server::cmd_mode(std::vector<std::string> &v, const int fd)
{
	if (v.size() < 2) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
	}
	if (v[1][0] == '#') {
		// channel mode
		// TODO channel mode
		if (_channels.find(v[1]) == _channels.end()) {
			_users[fd].send_msg(ERR_NOSUCHCHANNEL(_users[fd].get_nickname(), v[1]));
		} else {
			if (v.size() == )
		}

		
	} else {
		// user mode
		std::map<int, std::string>::iterator it = find_nickname(v[1]);
		if (it == _nicknames.end()) {
			_users[fd].send_msg(ERR_NOSUCHNICK(_users[fd].get_nickname(), v[1]));
		} else {
			if (v[1] != _nicknames[fd]) {
				_users[fd].send_msg(ERR_USERSDONTMATCH(_users[fd].get_nickname()));
			} else {
				if (v.size() == 2) {
					_users[fd].send_msg(RPL_UMODEIS(_users[fd].get_nickname(), _users[fd].get_mode()));
				} else {
					int flag = 0;
					for (size_t i = 0; i < v[2].size(); ++i) {
						if (v[2][i] == '+') { flag = 0; continue; }
						else if (v[2][i] == '-') { flag = 1; continue; }
						if (v[2][i] == 'i') {
							_users[fd].set_mode(INVISIBLE_USER_MODE, flag);
						} else if (v[2][i] == 'o') {
							_users[fd].set_mode(OPER_USER_MODE, flag);
						} else if (v[2][i] == 'O') {
							_users[fd].set_mode(LOCAL_OPER_USER_MODE, flag);
						} else if (v[2][i] == 'r') {
							_users[fd].set_mode(REGISTERED_USER_MODE, flag);
						} else if (v[2][i] == 'w') {
							_users[fd].set_mode(WALLOPS_USER_MODE, flag);
						} else {
							_users[fd].send_msg(ERR_UMODEUNKNOWNFLAG(_users[fd].get_nickname()));
							return ;
						}
					}
					_users[fd].send_msg(":ircserv MODE " + v[1] + " " + v[2] + "\n");
				}
			}
		}
	}
}


void Server::cmd_privmsg(std::vector<std::string> &v, const int fd)
{
	//TODO - ERR 처리
	//TODO - 타겟이 channel일 때
	std::vector<std::string>::iterator it = v.begin();
	for (; it != v.end(); ++it) {
		std::cout << *it << '\n';
	}
	/**
	 * ERR_NOSUCHNICK (no such nick/channel)
	 * 
	*/
	std::map<int, std::string>::iterator itm = find_nickname(v[1]);
	_users[itm->first].send_msg(":" + _users[fd].get_nickname()
	 + " PRIVMSG " + itm->second + " :" + v[2] + "\n");


}

void Server::cmd_notice(std::vector<std::string> &v, const int fd)
{
	//TODO - ERR 처리 (ERR 문구 send하면 안됨)

	std::map<int, std::string>::iterator itm = find_nickname(v[1]);
	_users[itm->first].send_msg(":" + _users[fd].get_nickname()
	 + " NOTICE " + itm->second + " :" + v[2] + "\n");
}

void Server::cmd_kill(std::vector<std::string> &v, const int fd)
{
	
}



