//
// Created by Павел Петров on 11/04/2020.
//

#ifndef BLACK_JACK_GAMESETTINGS_H
#define BLACK_JACK_GAMESETTINGS_H
#include <list>

class GameSettings {
    int players_money_amount;
    int min_bet;
    int bet_range;
public:
    GameSettings();

    int GetPlayersMoney();
    int GetMinimumBet();
    int GetBetRange();
};

#endif //BLACK_JACK_GAMESETTINGS_H