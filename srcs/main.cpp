#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include "Server.hpp"
#include "Util.hpp"

/*
./ircserv <port> <password>
*/

#define PORT 6667


int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>\n";
		exit(1);
	}
	std::cout << "port : " << argv[1] << "  password : " <<  argv[2] << '\n';

/*
	argv[1] 검사
*/

	Server server(6667, std::string(argv[2]));
	server.execute();


	// {
	// 	std::vector<std::string> v = ft_split(std::string(buffer), '\n');
	// 	for (std::vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
	// 		std::cout << *it << '\n';
	// 		std::vector<std::string> vc = ft_split(*it, ' ');
	// 		std::cout << vc[0] << '\n';
	// }
		// write(client_socket, "CAP * LS", strlen("hello!!!"));
		// char p[1000] = ":ircserv 001 han :Welcome to the Internet Relay Network han\r\n";
		// if (i == 1)
		// 	write(client_socket, p, strlen(p));


	return 0;
}