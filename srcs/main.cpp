#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include "Server.hpp"
#include "Util.hpp"

/* ./ircserv <port> <password> */

int main(int argc, char *argv[]) {
	if (argc != 3) {
		std::cerr << "Usage : ./ircserv <port> <password>\n";
		return 1;
	}
	int port = convertPort(std::string(argv[1]));
	if (port == -1) {
		std::cerr << "port is not available\n";
		return 1;
	}

	Server server(port, std::string(argv[2]));
	server.execute();

	return 0;
}