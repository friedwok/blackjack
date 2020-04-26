#include <iostream>
#include "client_processor.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <cstring>

Client_Processor::Client_Processor(int ls)
{
    listen_sock = ls;
    players_count = 0;
    int *game_params = this->GetGameParameters(ls);
    money_in_hand = game_params[0];
    min_bet = game_params[1];
    range_bet = game_params[2];

}

//returns an array of three numbers - players_money, minimum_bet, bet_range
int *Client_Processor::GetGameParameters(int ls)
{
    std::cout << "GGP" << std::endl;
    int len;
    fd_set readfds;
    char buf[1024];
    std::vector<std::string> params;
    int *game_params = new int[3];

    FD_ZERO(&readfds);
    FD_SET(ls, &readfds);
    select(ls+1, &readfds, NULL, NULL, NULL);
    if(FD_ISSET(ls, &readfds)) {
        len = recv(listen_sock, buf, sizeof(buf) - 1, 0);
        buf[len] = 0;
        if(len == -1) {
            std::cerr << "Recieve error" << std::endl;
            exit(1);
        }
        std::string str(buf);
        if(!len) {
            std::cout << "Server configuration error, restart client" << std::endl;
            exit(0);
        } else if(str == "All tables are busy, wait until the game ends\0") {
            std::cout << str << std::endl;
            exit(0);
        } else {
            std::string str(buf, len);
            boost::split(params, str, boost::is_any_of(" "));
        }

        for(int i = 0; i < 3; i++) {
            game_params[i] = std::stoi(params[i]);
            std::cout << "Param " << i << " : " << game_params[i] << std::endl;
        }
    }
    return game_params;
}

void Client_Processor::PreGameProcessor()
{
    fd_set readfds;
    int len;
    char tmp_buf[1024];
    std::vector<std::string> results;

    while(true) {
        FD_ZERO(&readfds);
        FD_SET(0, &readfds);
        FD_SET(listen_sock, &readfds);
        select(listen_sock+1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(listen_sock, &readfds)) {
            len = recv(listen_sock, tmp_buf, sizeof(tmp_buf) - 1, 0);
            tmp_buf[len] = 0;
            if(len == -1) {
                std::cerr << "Recieve error" << std::endl;
                exit(1);
            } else if(len == 0) {
                std::cout << "Server is down" << std::endl;
                exit(0);
            }
            std::cout << tmp_buf << std::endl;
            std::string str(tmp_buf);
            boost::split(results, str, boost::is_any_of(" "));
            if(results[0] == "Players" and results[1] == "connected:") {
                if(results[2][0] == results[2][2]) {
                    //std::cout << "GGP" << std::endl;
                    //this->GetGameParameters(listen_sock);
                    break;
                }
            }
        }

        if(FD_ISSET(0, &readfds)) {
            len = read(0, tmp_buf, sizeof(tmp_buf) - 1);
            tmp_buf[len-1] = 0;
            if(len == -1) {
                std::cerr << "Error by recieveing from c_in" << std::endl;
            }
            //потом доделать отключение от сервера в pre-game стадии
            //std::cout << tmp_buf << std::endl;
            //std::cout << strcmp(tmp_buf, "Disconnect") << std::endl;
            //std::cout << len << std::endl;
            if(strcmp(tmp_buf, "Disconnect") == 0) {
                shutdown(listen_sock, 2);
                close(listen_sock);
                exit(0);
            }
        }
    }
}

void Client_Processor::MainProcessor()
{
    fd_set readfds;
    int len;
    char tmp_buf[1024];
    int curr_state = State_1;

    while(curr_state != State_4) {
        FD_SET(0, &readfds);
        FD_SET(listen_sock, &readfds);
        select(listen_sock+1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(listen_sock, &readfds)) {
            len = recv(listen_sock, tmp_buf, sizeof(tmp_buf) - 1, 0);
            tmp_buf[len] = 0;
            if(len == -1) {
                std::cerr << "Recieve error" << std::endl;
            }
            this->Process(tmp_buf, len, curr_state);
        }
    }
}

Client_Processor::~Client_Processor()
{
    //shutdown(listen_sock, 2);
    close(listen_sock);
}

void Client_Processor::Process(char *buff, int len, int state)
{
    switch(state)
    {
        case State_1:
            this->Process_State_One(buff, len, state);
            break;
        case State_2:
            this->Process_State_Two(buff, len, state);
            break;
        case State_3:
            this->Process_State_Three(buff, len, state);
            break;
    }
}

void Client_Processor::Process_State_One(char *buff, int length, int state)
{
    std::cout << "First process started" << std::endl;
    if(!length) {
        std::cout << "Sorry, server is down" << std::endl;
        exit(0);
    }
}

void Client_Processor::Process_State_Two(char *buff, int length, int state)
{

}

void Client_Processor::Process_State_Three(char *buff, int length, int state)
{

}