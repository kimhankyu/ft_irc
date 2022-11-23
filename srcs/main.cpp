#include <iostream>
#include <sys/socket.h>

/*
./ircserv <port> <password>
*/

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage : ./ircserv <port> <password>\n";
        exit(1);
    }
    std::cout << "port : " << argv[1] << "  password : " <<  argv[2] << '\n';



    return 0;
}