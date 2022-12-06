#include "Server.hpp"
#include "User.hpp"
#include "Reply.hpp"

void Server::cmd_pass(std::vector<std::string> v, int fd)
{
	if (v.size() != 2) {
		_users[fd].send_msg(ERR_NEEDMOREPARAMS(_users[fd].get_nickname(), v[0]));
	} else if (_users[fd].get_registered()) {
		_users[fd].send_msg(ERR_ALREADYREGISTRED(_users[fd].get_nickname()));
	} else if (_password != v[1]) {
		_users[fd].send_msg(ERR_PASSWDMISMATCH(_users[fd].get_nickname()));
	}
}



// void cmd_nick()
// {

// }

// void cmd_user()
// {

// }