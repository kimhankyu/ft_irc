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

//TODO - QUIT


void Server::cmd_join(std::vector<std::string> &v, const int fd)
{
	if (v.size() < 2 || v.size() > 3) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
		return ;
	}
	std::vector<std::string> chanList = ft_split(v[1], ',');
	std::vector<std::string> keyList;
	if (v.size() == 3) {
		keyList = ft_split(v[2], ',');
	}
	size_t keyIndex = 0;
	for (std::vector<std::string>::iterator it = chanList.begin(); it != chanList.end(); ++it) {
		if (_users[fd].is_channel_user(*it)) {
			if (v.size() == 3 && keyIndex < keyList.size()) ++keyIndex;
			continue;
		}
		if (_users[fd].get_channels_num() >= CHAN_USER_LIMIT) {
			_users[fd].send_msg(ERR_TOOMANYCHANNELS(_users[fd].get_nickname(), *it));
			if (v.size() == 3 && keyIndex < keyList.size()) ++keyIndex;
			continue;
		}
		if (_channels.find(*it) != _channels.end()) { // 존재하면
			if (_channels[*it].is_mode(INVITE_ONLY_CHANNEL_MODE)) {
				_users[fd].send_msg(ERR_INVITEONLYCHAN(_users[fd].get_nickname(), *it));
				if (v.size() == 3 && keyIndex < keyList.size()) ++keyIndex;
				continue;
			}
			if (_channels[*it].is_mode(KEY_CHANNEL_MODE)) {
				if (v.size() != 3 || !_channels[*it].is_key_same(keyList[keyIndex])) {
					_users[fd].send_msg(ERR_BADCHANNELKEY(_users[fd].get_nickname(), *it));
					if (v.size() == 3 && keyIndex < keyList.size()) ++keyIndex;
					continue;
				}
			}
			_channels[*it].add_user(_users[fd]);
		} else { // 새로 생성
			// TODO - channel name check
			_channels[*it] = Channel(*it, _users[fd]);
			if (v.size() == 3 && keyIndex < keyList.size()) ++keyIndex;
		}
		_users[fd].add_channel(*it);
		_users[fd].send_msg(":" + _users[fd].get_nickname() + " JOIN " + *it + "\n");
		if (_channels[*it].get_topic() != "") {
			_users[fd].send_msg(RPL_TOPIC(_users[fd].get_nickname(), *it, _channels[*it].get_topic()));
		}
		// RPL_NAMREPLY
		// RPL_ENDOFNAMES
	}
}

void Server::cmd_part(std::vector<std::string> &v, const int fd)
{
	if (v.size() < 2 || v.size() > 3) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
		return ;
	}
	std::vector<std::string> chanList = ft_split(v[1], ',');
	std::vector<std::string> reasonList;
	size_t reasonIndex = 0;
	if (v.size() == 3) {
		reasonList = ft_split(v[2], ',');
	}
	for (std::vector<std::string>::iterator it = chanList.begin(); it != chanList.end(); ++it) {
		if (_channels.find(*it) != _channels.end()) {
			if (_channels[*it].is_user(_users[fd])) {
				std::string msg = ":" + _users[fd].get_fullname() + " PART " + *it;
				if (v.size() == 3 && reasonIndex < reasonList.size()) {
					msg += " " + reasonList[reasonIndex++];
				}
				_channels[*it].send_msg(msg + "\n");
				_channels[*it].del_user(_users[fd]);
				_users[fd].remove_channel(*it);
				if (_channels[*it].get_user_num() == 0) {
					_channels.erase(*it);
				}
			} else {
				_users[fd].send_msg(ERR_NOTONCHANNEL(_users[fd].get_nickname(), *it));
			}
		} else {
			_users[fd].send_msg(ERR_NOSUCHCHANNEL(_users[fd].get_nickname(), *it));
		}
	}
}

void Server::cmd_topic(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 2 && v.size() != 3) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
		return;
	}
	if (_channels.find(v[0]) == _channels.end()) {
		_users[fd].send_msg(ERR_NOSUCHCHANNEL(_users[fd].get_nickname(), v[1]));
		return;
	}
	if (!_channels[v[1]].is_user(_users[fd])) {
		_users[fd].send_msg(ERR_NOTONCHANNEL(_users[fd].get_nickname(), v[1]));
		return;
	}
	if (v.size() == 2) {
		std::string topic = _channels[v[1]].get_topic();
		if (topic == "") {
			_users[fd].send_msg(RPL_NOTOPIC(_users[fd].get_nickname(), v[1]));
		} else {
			_users[fd].send_msg(RPL_TOPIC(_users[fd].get_nickname(), v[1], topic));
		}
	} else {
		if (_channels[v[1]].is_mode(PROTECTED_TOPIC_MODE)) {
			if (!_channels[v[1]].is_operator(_users[fd])) {
				_users[fd].send_msg(ERR_CHANOPRIVSNEEDED(_users[fd].get_nickname(), v[1]));
				return;
			}
		}
		std::string topic = v[2].substr(0);
		_channels[v[1]].set_topic(topic);
		_channels[v[1]].send_msg(":" + _users[fd].get_nickname() + " TOPIC " + v[1] + " " + v[2] + "\n");
	}
}

void Server::cmd_invite(std::vector<std::string> &v, const int fd)
{
	if (v.size() != 3) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
		return;
	}
	if (_channels.find(v[0]) == _channels.end()) {
		_users[fd].send_msg(ERR_NOSUCHCHANNEL(_users[fd].get_nickname(), v[1]));
		return;
	}
	std::map<int, std::string>::iterator it = find_nickname(v[1]);
	if (it == _nicknames.end()) {
		_users[fd].send_msg(ERR_NOSUCHNICK(_users[fd].get_nickname(), v[1]));
		return;
	}
	if (!_channels[v[2]].is_user(_users[fd])) {
		_users[fd].send_msg(ERR_NOTONCHANNEL(_users[fd].get_nickname(), v[2]));
		return;
	}
	if (_channels[v[2]].is_mode(INVITE_ONLY_CHANNEL_MODE)) {
		if (!_channels[v[2]].is_operator(_users[fd])) {
			_users[fd].send_msg(ERR_CHANOPRIVSNEEDED(_users[fd].get_nickname(), v[2]));
			return;
		}
	}
	if (_channels[v[2]].is_user(_users[(*it).first])) {
		_users[fd].send_msg(ERR_USERONCHANNEL(_users[fd].get_nickname(), v[1], v[2]));
		return;
	}
	_users[fd].send_msg(RPL_INVITING(_users[fd].get_nickname(), v[1], v[2]));
	_users[(*it).first].send_msg(":" + _users[fd].get_nickname() + " INVITE " + v[1] + " " + v[2] + "\n");
}

void Server::cmd_kick(std::vector<std::string> &v, const int fd)
{
	if (v.size() < 3 || v.size() > 4) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
		return;
	}
	if (_channels.find(v[1]) == _channels.end()) {
		_users[fd].send_msg(ERR_NOSUCHCHANNEL(_users[fd].get_nickname(), v[1]));
		return;
	}
	if (!_channels[v[1]].is_operator(_users[fd])) {
		_users[fd].send_msg(ERR_CHANOPRIVSNEEDED(_users[fd].get_nickname(), v[1]));
		return;
	}
	std::vector<std::string> userList = ft_split(v[2], ',');
	for (std::vector<std::string>::iterator it = userList.begin(); it != userList.end(); ++it) {
		if (!_channels[v[1]].is_user(_users[find_nickname(*it)->first])) {
			_users[find_nickname(*it)->first].send_msg(ERR_NOTONCHANNEL(_users[find_nickname(*it)->first].get_nickname(), v[1]));
			continue;
		}
		_channels[v[1]].del_user(_users[find_nickname(*it)->first]);
		_users[find_nickname(*it)->first].remove_channel(v[1]);
		std::string str = ":ircserv " + v[0] + " " + v[1] + " " + *it;
		if (v.size() == 4) {
			str += ":" + v[3];
		}
		_channels[v[1]].send_msg(str + "\n");
	}
}

void Server::cmd_mode(std::vector<std::string> &v, const int fd)
{
	if (v.size() < 2) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
		return;
	}
	if (v[1][0] == '#') {
		if (_channels.find(v[1]) == _channels.end()) {
			_users[fd].send_msg(ERR_NOSUCHCHANNEL(_users[fd].get_nickname(), v[1]));
			return ;
		}
		if (v.size() == 2) {
			std::string str = RPL_CHANNELMODEIS(_users[fd].get_nickname(), v[1],
				_channels[v[1]].get_mode());
			if (_channels[v[1]].is_mode(KEY_CHANNEL_MODE)) {
				str += " " + _channels[v[1]].get_key() + "\n";
			} else {
				str += "\n";
			}
			_users[fd].send_msg(str);
			return;
		}
		if (!_channels[v[1]].is_operator(_users[fd])) {
			_users[fd].send_msg(ERR_CHANOPRIVSNEEDED(_users[fd].get_nickname(), v[1]));
			return;
		}
		int flag = 0;
		for (size_t i = 0; i < v[2].size(); ++i) {
			if (v[2][i] == '+') { flag = 0; continue; }
			else if (v[2][i] == '-') { flag = 1; continue; }
			if (v[2][i] == 'i') {
				_channels[v[1]].set_mode(INVITE_ONLY_CHANNEL_MODE, flag);
			} else if (v[2][i] == 't') {
				_channels[v[1]].set_mode(PROTECTED_TOPIC_MODE, flag);
			} else if (v[2][i] == 'k') {
				if (v.size() < 4) {
					_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
					return;
				}
				_channels[v[1]].set_mode(KEY_CHANNEL_MODE, flag, v[3]);
			} else {
				_users[fd].send_msg(ERR_UMODEUNKNOWNFLAG(_users[fd].get_nickname()));
			}
		}
		std::string str = ":ircserv MODE " + v[1] + " " + v[2];
		if (v.size() >= 4) {
			for (size_t i = 3; i < v.size(); ++i)
				str += " " + v[i];
		}
		str += "\n";
		_channels[v[1]].send_msg(str);
	} else {
		std::map<int, std::string>::iterator it = find_nickname(v[1]);
		if (it == _nicknames.end()) {
			_users[fd].send_msg(ERR_NOSUCHNICK(_users[fd].get_nickname(), v[1]));
			return ;
		}
		if (v[1] != _nicknames[fd]) {
			_users[fd].send_msg(ERR_USERSDONTMATCH(_users[fd].get_nickname()));
			return;
		}
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

// TODO - CHANNAL로 보낼 때 중복으로 보내짐
void Server::cmd_privmsg(std::vector<std::string> &v, const int fd)
{
	if (v[1][0] == '#') {
		_channels[v[1]].send_msg(":" + _users[fd].get_nickname()
	 + " PRIVMSG " + v[1] + " :" + v[2] + "\n");
	} else {
		std::map<int, std::string>::iterator itm = find_nickname(v[1]);
		if (itm == _nicknames.end()) {
			_users[fd].send_msg(ERR_NOSUCHNICK(_users[fd].get_nickname(), v[1]));
			return;
		}
		_users[itm->first].send_msg(":" + _users[fd].get_nickname()
		+ " PRIVMSG " + itm->second + " :" + v[2] + "\n");
	}
}

void Server::cmd_notice(std::vector<std::string> &v, const int fd)
{
	if (v[1][0] == '#') {
		_channels[v[1]].send_msg(":" + _users[fd].get_nickname()
	 + " NOTICE " + v[1] + " :" + v[2] + "\n");
	} else {
		std::map<int, std::string>::iterator itm = find_nickname(v[1]);
		if (itm == _nicknames.end()) {
			return;
		}
		_users[itm->first].send_msg(":" + _users[fd].get_nickname()
		+ " NOTICE " + itm->second + " :" + v[2] + "\n");
	}
}

//TODO - cmd_kill
// void Server::cmd_kill(std::vector<std::string> &v, const int fd)
// {
	
// }


