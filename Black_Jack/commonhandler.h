//
// Created by Павел Петров on 2020-04-03.
//

#ifndef BLACK_JACK_COMMONHANDLER_H
#define BLACK_JACK_COMMONHANDLER_H

#include <string>
#include <cctype>
#include <utility>
#include "gamesettings.h"

class CommonHandler {
protected:
    //number of players in the game
    static int players;
    //connected players
    //static std::list<int> player_descriptors;
    static std::string no_places_message;
    //0 - pre-game; 1 - the game is going on;
    static int game_condition;
    //static std::pair<int, int> *players_state;
public:
    CommonHandler();

    //check if string is a number
    static bool is_number(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        while(it != s.end() and std::isdigit(*it)) ++it;
        return !s.empty() and it == s.end();
    }

    static void SetPlayers(int pls)
    {
        players = pls;
    }

    //breaks the connection
    void BreakConnection(int dscr);

    //send message to all players
    void SendAll(char *message, int size, std::list<int> *players_list);

    //send message about connected players
    void SendAll_InfoConnectedPlayers(std::list<int> *players_list);

    //send game parameters
    void SendGameParameters(int fd, GameSettings *game_settings);

    //get the number of connected players
    //int GetNumOfConnectedGuys();

    //returns the game condition
    int GameState();
};

#endif //BLACK_JACK_COMMONHANDLER_H
