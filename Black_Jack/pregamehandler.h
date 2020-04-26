//
// Created by Павел Петров on 12/04/2020.
//

#ifndef BLACK_JACK_PREGAMEHANDLER_H
#define BLACK_JACK_PREGAMEHANDLER_H

#include <list>
#include "gamesettings.h"
#include "commonhandler.h"

class PreGameHandler : public CommonHandler {
public:
    PreGameHandler();

    //add the come descriptor to the list
    void AddAcceptedDescriptor(int dscr, std::list<int> *players_list);

    //returns -1 if there are no more places
    int HandleAcceptedPlayer(int dscr_came, std::list<int> *players_list, GameSettings *game_settings);

    //pregame handler
    int PreGameHandle(int sym_read, std::list<int>::iterator& it, int fd, std::list<int> *players_list);

    //start an event loop
    //void StartEventLoop(int listen_sock, std::list<int> *players, PreGameHandler& pre_game_handler);

    void StartEventLoop(int listen_sock, std::list<int> *players, GameSettings *game_settings);
};

#endif //BLACK_JACK_PREGAMEHANDLER_H
