#include "pregamehandler.h"
#include "sys/socket.h"
#include <iostream>
#include <unistd.h>


PreGameHandler::PreGameHandler() : CommonHandler() {}

void PreGameHandler::AddAcceptedDescriptor(int dscr, std::list<int> *players_list)
{
    players_list->push_back(dscr);
}

int PreGameHandler::HandleAcceptedPlayer(int dscr_came, std::list<int> *players_list, GameSettings *game_settings)
{
    int n;
    if(players_list->size() == players) {
        n = send(dscr_came, no_places_message.c_str(), no_places_message.size() + 1, 0);
        if(n == -1) {
            std::cerr << "Send error" << std::endl;
        }
        shutdown(dscr_came, 2);
        close(dscr_came);
    } else {
        this->AddAcceptedDescriptor(dscr_came, players_list);
        this->SendGameParameters(dscr_came, game_settings);
        this->SendAll_InfoConnectedPlayers(players_list);
        if(players_list->size() == players) {
            game_condition = 1;
            char tmp_buf[30] = "The game has begun!\n";
            std::cout << tmp_buf;
            tmp_buf[strlen(tmp_buf)] = 0;
            //this->SendAll(tmp_buf, strlen(tmp_buf) + 1, players_list);
            //this->SendGameParameters(dscr_came, game_settings);
            return 1;
        }
    }

    return 0;
}

int PreGameHandler::PreGameHandle(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list)
{
    //потом доделать обработку разных команд(после else); help, rules, wrong command и т.д.
    std::cout << "PreGameHandle!" << std::endl;
    if(!sym_read) {
        this->BreakConnection(fd);
        players_list->remove(fd);
        this->SendAll_InfoConnectedPlayers(players_list);
        return 1;
    } else {
        this->BreakConnection(fd);
        ++it;
        players_list->remove(fd);
        this->SendAll_InfoConnectedPlayers(players_list);
        return 2;
    }
}

//void PreGameHandler::StartEventLoop(int listen_sock, std::list<int> *players, PreGameHandler& pre_game_handler)
void PreGameHandler::StartEventLoop(int listen_sock, std::list<int> *players, GameSettings *game_settings)
{
    int dscr_came;
    struct sockaddr addr_for_accept;
    socklen_t addrlen = sizeof(struct sockaddr);
    for(;;) {
        fd_set readfds;
        int max_d = listen_sock;

        FD_ZERO(&readfds);
        FD_SET(listen_sock, &readfds);

        std::list<int>::iterator it;
        int fd;
        for(it = players->begin(); it!= players->end(); it++) {
            fd = *it;
            FD_SET(fd, &readfds);
            if(fd > max_d)
                max_d = fd;
        }
        int res = select(max_d+1, &readfds, NULL, NULL, NULL);
        if(res < 1) {
            std::cerr << "Select error..." << std::endl;
        }
        if(FD_ISSET(listen_sock, &readfds)) {
            dscr_came = accept(listen_sock, &addr_for_accept, &addrlen);
            if(dscr_came < 0) {
                std::cerr << "Accept error..." << std::endl;
                exit(1);
            }
            if(this->HandleAcceptedPlayer(dscr_came, players, game_settings)) {
                return;
            }
        }

        int rs;
        char tmp_buf[1024];
        it = players->begin();
        while(it != players->end()) {
            fd = *it;
            if(FD_ISSET(fd, &readfds)) {
                rs = recv(fd, tmp_buf, sizeof(tmp_buf) - 1, 0);
                tmp_buf[rs] = 0;
                if(rs < 0) {
                    std::cout <<"Goddamn...!!!" << std::endl;
                    exit(1);
                }
                //int ret = pre_game_handler.PreGameHandle(rs, it, fd, players);
                int ret = this->PreGameHandle(rs, it, fd, players);
                if (ret == 1) {
                    break;
                } else if(ret == 2){
                    continue;
                }
            }
            ++it;
        }
    }
}