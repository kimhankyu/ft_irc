#include <iostream>
#include <fcntl.h>
#include "Server.hpp"
#include "Util.hpp"
#include "Reply.hpp"

Server::Server(int port, std::string password)
: _port(port), _password(password)
{
	socket_init();
	pollfd_init();


}

void Server::socket_init()
{
	_server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (_server_socket == -1) {
		perror("socket fail");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
		perror("setsocketopt fail");
		exit(EXIT_FAILURE);
	}
	_server_addr.sin_family = AF_INET;
	_server_addr.sin_addr.s_addr = INADDR_ANY;
	_server_addr.sin_port = htons(_port);

	if (fcntl(_server_socket, F_SETFL, O_NONBLOCK) < 0) {
		perror("fcntl fail");
		exit(EXIT_FAILURE);
	}
	if (bind(_server_socket, (struct sockaddr*)&_server_addr, sizeof(_server_addr)) == -1) {
		perror("bind fail");
		exit(EXIT_FAILURE);
	}
	if (listen(_server_socket, 20) == -1) {
		perror("listen fail");
		exit(EXIT_FAILURE);
	}
}

void Server::pollfd_init()
{
	create_poll(_server_socket);
}

void Server::create_poll(int fd)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = POLLIN;
	_fds.push_back(pfd);
	User user(fd);
	_users[fd] = user;
}

void Server::accept_client()
{
	struct sockaddr_in client_addr;
	socklen_t cli_len = sizeof(client_addr);

	_client_socket = accept(_server_socket, (struct sockaddr *)&client_addr, &cli_len);
	if (_client_socket == -1) {
		perror("accept fail");
		exit(EXIT_FAILURE);
	}
	create_poll(_client_socket);
}

void Server::echo_message()
{
	int n;
	char buff[MSG_LEN + 1] = {0, };

	for (std::vector<pollfd>::size_type i = 1; i < _fds.size(); ++i) {
		if (_fds[i].revents & POLLIN) {
			if ((n = read(_fds[i].fd, buff, sizeof(buff))) <= 0) {
				if (n  == -1) {
					perror("read fail");
					exit(EXIT_FAILURE);
				} else if (n == 0) {
					//quit
					std::cout << "n == 0\n";

				}
			} else {
				buff[n] = '\0';
				//command
				std::vector<std::string> v = ft_split(std::string(buff), '\n');
				for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
					execute_command(*it, _fds[i].fd);

				}

				// write
				std::cout << buff;


			}


		}
	}
}

void Server::execute_command(std::string str, const int fd)
{
	std::vector<std::string> ve;
	size_t pos = str.find_first_of(':');
	if (pos == str.npos) {
		ve.push_back(str);
	} else {
		ve.push_back(str.substr(0, pos));
		ve.push_back(str.substr(pos + 1));
	}
	std::vector<std::string> v = ft_split(ve[0], ' ');
	if (ve.size() != 1) {
		v.push_back(ve[1]);
	}

	std::vector<std::string>::iterator it = v.end() - 1;
	pos = it->find("\r");
	*it = it->substr(0, pos);

	if (v[0] == "PASS") {
		cmd_pass(v, fd);
	} else if (v[0] == "NICK") {
		cmd_nick(v, fd);
	} else if (v[0] == "USER") {
		cmd_user(v, fd);
	} else if (v[0] == "PING") {
		cmd_ping(v, fd);
	} else if (v[0] == "PRIVMSG") {
		cmd_privmsg(v, fd);
	} else if (v[0] == "NOTICE") {
		cmd_notice(v, fd);
	} else if (v[0] == "MODE") {
		cmd_mode(v, fd);
	} else if (v[0] == "OPER") {
		cmd_oper(v, fd);
	}





}

void Server::execute()
{
	int n;
	while (true) {
		n = poll(_fds.data(), _fds.size(), TIMEOUT);
//TODO - poll err check

		if (_fds[0].revents & POLLIN) { accept_client(); }
		echo_message();
	}
}

Server::~Server()
{
	close(_server_socket);
	close(_client_socket);
}
