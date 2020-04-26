//
// Created by Павел Петров on 2020-04-04.
//

#ifndef BLACK_JACK_CLIENT_CLIENT_PROCESSOR_H
#define BLACK_JACK_CLIENT_CLIENT_PROCESSOR_H

class Client_Processor {
    int listen_sock;
    int players_count;
    enum States {
        State_1,    //Making bets, waiting for all players
        State_2,    //players draw cards and make decisions, in turn
        State_3,    //look at the dealer cards, send chip's count to the server
        State_4,    //End
    };
    int money_in_hand;
    int min_bet;
    int range_bet;
public:
    Client_Processor(int ls);
    int *GetGameParameters(int ls);
    void PreGameProcessor();
    void MainProcessor();
    void Process(char *buff, int length, int state);
    void Process_State_One(char *buff, int length, int state);
    void Process_State_Two(char *buff, int length, int state);
    void Process_State_Three(char *buff, int length, int state);
    //void MakeBet();
    ~Client_Processor();
};

#endif //BLACK_JACK_CLIENT_CLIENT_PROCESSOR_H
