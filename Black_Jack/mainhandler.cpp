#include "commonhandler.h"
#include "mainhandler.h"
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>


MainHandler::MainHandler() : CommonHandler {}
{
    current_state = State_1;
}

int MainHandler::GetState()
{
    return current_state;
}

void MainHandler::SetPairs(std::list<int> *players_list)
{
    std::list<int>::iterator it = players_list->begin();
    for(int i = 0; i < players_list->size(); i++)
    {
        players_state.push_back(std::make_pair(*it, 0));
        ++it;
    }
}

void MainHandler::StartEventLoop(int listen_sock, std::list<int> *players)
{
    int dscr_came;
    struct sockaddr addr_for_accept;
    socklen_t addrlen = sizeof(struct sockaddr);
    for (;;) {
        fd_set readfds;
        int max_d = listen_sock;

        FD_ZERO(&readfds);
        FD_SET(listen_sock, &readfds);

        std::list<int>::iterator it;
        int fd;
        for (it = players->begin(); it != players->end(); it++) {
            fd = *it;
            FD_SET(fd, &readfds);
            if (fd > max_d)
                max_d = fd;
        }
        std::cout << "Presel" << std::endl;
        int res = select(max_d + 1, &readfds, NULL, NULL, NULL);
        std::cout << "Aftersel" << std::endl;
        if (res < 1) {
            std::cerr << "Select error..." << std::endl;
        }
        if (FD_ISSET(listen_sock, &readfds)) {
            dscr_came = accept(listen_sock, &addr_for_accept, &addrlen);
            if (dscr_came < 0) {
                std::cerr << "Accept error..." << std::endl;
                exit(1);
            }
            int n = send(dscr_came, no_places_message.c_str(), no_places_message.size() + 1, 0);
            if (n == -1) {
                std::cerr << "Send error" << std::endl;
            }
            shutdown(dscr_came, 2);
            close(dscr_came);
        }

        int rs;
        char tmp_buf[1024];
        it = players->begin();
        while(it != players->end()) {
            fd = *it;
            if(FD_ISSET(fd, &readfds)) {
                rs = recv(fd, tmp_buf, sizeof(tmp_buf) - 1, 0);
                tmp_buf[rs] = 0;
                if (rs < 0) {
                    std::cout << "Recieve error" << std::endl;
                    exit(1);
                }
                this->Handle(rs, it, fd, players);
            }
        }
    }
}

void MainHandler::Handle(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list)
{
    switch(current_state)
    {
        case State_1:
            HandleStateOne(sym_read, it, fd, players_list);
            break;
        case State_2:
            HandleStateTwo(sym_read, it, fd, players_list);
            break;
        case State_3:
            HandleStateThree(sym_read, it, fd, players_list);
            break;
        default:
            std::cerr << "Error with handling, current state = " << current_state << std::endl;
            break;
    }
}

void MainHandler::HandleStateOne(int sym_read, std::list<int>::iterator &it, int fd, std::list<int> *players_list)
{
    std::cout << "First handler started!" << std::endl;

}

void MainHandler::HandleStateTwo(int sym_read, std::list<int>::iterator &it, int fd, std::list<int> *players_list)
{

}

void MainHandler::HandleStateThree(int sym_read, std::list<int>::iterator &it, int fd, std::list<int> *players_list)
{

}

MainHandler::~MainHandler()
{

}
