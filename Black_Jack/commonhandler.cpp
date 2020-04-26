#include <iostream>
#include <list>
#include "commonhandler.h"
#include <unistd.h>
#include <sys/socket.h>
#include <iterator>
#include <utility>
#include "gamesettings.h"

int CommonHandler::players = 0;
std::string CommonHandler::no_places_message = "All tables are busy, wait until the game ends\0";
int CommonHandler::game_condition = 0;

CommonHandler::CommonHandler() {}

void CommonHandler::SendAll(char *message, int size, std::list<int> *players_list)
{
    std::list<int>::iterator it;
    int n;
    for(it = players_list->begin(); it != players_list->end(); ++it) {
        message[size] = 0;
        n = send(*it, message, size + 1, 0);
        if(n == -1) {
            std::cerr << "Send error" << std::endl;
        }
    }
}

void CommonHandler::SendAll_InfoConnectedPlayers(std::list<int> *players_list)
{
    std::string str = "Players connected: ";
    str += std::to_string(players_list->size());
    str += std::string("/");
    str += std::to_string(players);
    char *str_send = new char[str.size()];
    strcpy(str_send, str.c_str());
    std::cout << str_send << std::endl;
    this->SendAll(str_send, str.size(), players_list);
    delete [] str_send;
}

//void CommonHandler::SendGameParameters(std::list<int> *players_list, GameSettings *gameSettings)
void CommonHandler::SendGameParameters(int fd, GameSettings *gameSettings)
{
    std::string str_send = std::to_string(gameSettings->GetPlayersMoney());
    str_send += " ";
    str_send += std::to_string(gameSettings->GetMinimumBet());
    str_send += " ";
    str_send += std::to_string(gameSettings->GetBetRange());

    char *cstr_send = new char[str_send.size()];
    strcpy(cstr_send, str_send.c_str());
    int n = send(fd, cstr_send, str_send.size(), 0);
    if(n == -1) {
        std::cerr << "Send error" << std::endl;
    }
}

void CommonHandler::BreakConnection(int dscr)
{
    shutdown(dscr, 2);
    close(dscr);
}

int CommonHandler::GameState()
{
    return game_condition;
}