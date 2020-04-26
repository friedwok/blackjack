//
// Created by Павел Петров on 12/04/2020.
//

#ifndef BLACK_JACK_MAINHANDLER_H
#define BLACK_JACK_MAINHANDLER_H

#include <list>
#include "commonhandler.h"

class MainHandler : public CommonHandler {
    enum States {
        State_1, // Players making bets, last until all players make bets
        State_2, // dealer deals cards and then waiting for answer
        State_3, // dealer opens second card and takes additional
        State_4, // After the game, the server checks the player's chips
    };
    int current_state;
    std::list< std::pair<int, int> > players_state;
public:
    MainHandler();

    //returns the current state
    int GetState();

    //set pair values
    void SetPairs(std::list<int> *players_list);

    //start an event loop, game stage
    void StartEventLoop(int listen_sock, std::list<int> *players);

    //handler in the main cycle
    void Handle(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list);

    void HandleStateOne(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list);

    void HandleStateTwo(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list);

    void HandleStateThree(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list);

    ~MainHandler();
};

#endif //BLACK_JACK_MAINHANDLER_H
