#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <cstdlib>
#include <string>
#include <cctype>
#include <arpa/inet.h>
#include <unistd.h>
#include "client_processor.h"

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while(it != s.end() and std::isdigit(*it)) ++it;
    return !s.empty() and it == s.end();
}

//one arg - port
int main(int argc, char **argv) {
    int ls, port;
    struct sockaddr_in addr;
    char serv_ip[20];
    bool ip_valid = false;

    if (argc != 2) {
        std::cout << "Programm has one argument - port" << std::endl;
    }

    if (is_number(argv[1])) {
        port = std::atoi(argv[1]);
    } else {
        std::cout << "Wrong parameter" << std::endl;
    }

    ls = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    while (!ip_valid) {
        std::cout << "Input IP address of the server" << std::endl;
        scanf("%s", serv_ip);
        if (!inet_aton(serv_ip, &(addr.sin_addr))) {
            std::cout << "IP address is not valid" << std::endl;
        } else {
            ip_valid = true;
        }
    }

    if(0 != connect(ls, (struct sockaddr*) &addr, sizeof(addr))) {
        std::cerr << "Connection error" << std::endl;
        exit(1);
    }

    std::cout << "Connection established" << std::endl;

    Client_Processor client_processor(ls);
    client_processor.PreGameProcessor();
    client_processor.MainProcessor();

    return 0;
}