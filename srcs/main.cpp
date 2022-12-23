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


// TODO - argv[1] 검사

	Server server(6667, std::string(argv[2]));
	server.execute();




	return 0;
}