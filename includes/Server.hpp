#ifndef SERVER_HPP
#define SERVER_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <unistd.h>
#include <cerrno>
#include <string>
#include <vector>
#include <map>
#include "User.hpp"

#define TIMEOUT 1000
#define MSG_LEN 512

class Server
{
private:
	int _port;
	int _server_socket;
	int _client_socket;
	struct sockaddr_in _server_addr;

	std::string _password;
	std::vector<struct pollfd> _fds;
	std::map<int, User> _users;
	std::map<int, std::string> _nicknames;


	void socket_init();
	void pollfd_init();
	void create_poll(int fd);
	void accept_client();
	void echo_message();
	void execute_command(std::string str, const int fd);
	/* Command.cpp */
	void cmd_pass(std::vector<std::string> &v, const int fd);
	void cmd_nick(std::vector<std::string> &v, const int fd);
	bool find_nickname(std::string str);
	void cmd_user(std::vector<std::string> &v, const int fd);
	void cmd_pong(std::vector<std::string> &v, const int fd);






public:
	Server(int port, std::string password);
	void execute();
	~Server();
};





#endif
