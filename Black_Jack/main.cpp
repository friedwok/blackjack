#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <list>
#include <sys/select.h>
#include "commonhandler.h"
#include <stdlib.h>
#include "gamesettings.h"
#include "pregamehandler.h"
#include "mainhandler.h"

//argv[1] - players_count; argv[2] - number of port;
int main(int argc, char **argv)
{
    if(argc < 3) {
        std::cerr << "Invalid arguments: ./game_serv [players_count] [number of port]" << std::endl;
        exit(1);
    }

    int ls, port;
    struct sockaddr_in addr;
    int opt = 1;
    std::list<int> players;
    PreGameHandler pre_game_handler;
    MainHandler main_handler;
    GameSettings game_settings;

    if(CommonHandler::is_number(argv[2]) and CommonHandler::is_number(argv[1])) {
        port = atoi(argv[2]);
        CommonHandler::SetPlayers(atoi(argv[1]));
    } else {
        std::cerr << "Invalid arguments" << std::endl;
    }

    ls = socket(AF_INET, SOCK_STREAM, 0);
    if(ls < 0) {
        std::cerr << "Socket returned " << ls << std::endl;
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(0 != bind(ls, (struct sockaddr*) &addr, sizeof(addr))) {
        std::cerr << "Bind error, check port maybe..." << std::endl;
        exit(1);
    }

    if(-1 == listen(ls, 5)) {
        std::cerr << "Listen error" << std::endl;
    }

    //pre_game_handler.StartEventLoop(ls, &players, pre_game_handler);
    pre_game_handler.StartEventLoop(ls, &players, &game_settings);
    main_handler.SetPairs(&players);
    main_handler.StartEventLoop(ls, &players);

    return 0;
}
